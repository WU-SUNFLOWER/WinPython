#include "interpreter.hpp"
#include "FrameObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include <cstdlib>
#include "PyObject.hpp"
#include "Block.hpp"
#include "nativeFunctions.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "MethodKlass.hpp"
#include "PyMethod.hpp"
#include "StringKlass.hpp"

Interpreter::Interpreter() {
    _curFrame = nullptr;
    _retValue = nullptr;
    _buildins = new PyObjectMap();

    _buildins->set(new PyString("True"), Universe::PyTrue);
    _buildins->set(new PyString("False"), Universe::PyFalse);
    _buildins->set(Universe::PyNone, Universe::PyNone);
    
#define PackNativeFunc(f) (new PyFunction(reinterpret_cast<NativeFuncPointer>(f)))

    _buildins->set(new PyString("len"), PackNativeFunc(NativeFunction::len));

    PyObjectMap* dict_str = StringKlass::getInstance()->getKlassDict();
    dict_str->set(new PyString("upper"), 
        PackNativeFunc(NativeFunction::string_upper));
}

#define POP() (_curFrame->popFromStack())
#define PUSH(x) (_curFrame->pushToStack(x))
#define POP_BLOCK() (_curFrame->_blockStack->pop())
#define PUSH_BLOCK(type, target, level) \
    (_curFrame->_blockStack->push({type, target, level}))
#define PC (_curFrame->pc)
#define Consts (_curFrame->_consts)
#define StackLength (_curFrame->_stack->getLength())

void Interpreter::run(CodeObject* codeObject) {
    
    _curFrame = new FrameObject(codeObject);

    while (_curFrame != nullptr && _curFrame->hasMoreCode()) {
        uint8_t op_code = _curFrame->getOpCode();
        bool hasArugment = op_code >= ByteCode::Have_Argument;
        uint16_t op_arg = -1;

        // ���ָ��Σ���һ��ȡ����ȡ��ʱ��ע����С����
        if (hasArugment) {
            op_arg = _curFrame->getOpArgument();
        }
        
        PyObject* rhs, * lhs;

        switch (op_code) {
            case ByteCode::Pop_Top:
                POP();
                break;

            case ByteCode::Load_Const:
                PUSH(Consts->get(op_arg));
                break;

            case ByteCode::Print_Item:
                lhs = POP();
                lhs ->print();
                break;

            case ByteCode::Print_NewLine:
                putchar('\n');
                break;

            case ByteCode::Binary_Add:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                PUSH(lhs->add(rhs));
                break;

            case ByteCode::Compare_Op: {
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                switch (op_arg) {
                    case CompareCondition::Less:
                        PUSH(lhs->less(rhs));
                        break;
                    case CompareCondition::Less_Equal:
                        PUSH(lhs->less_equal(rhs));
                        break;
                    case CompareCondition::Equal:
                        PUSH(lhs->equal(rhs));
                        break;
                    case CompareCondition::Greater_Equal:
                        PUSH(lhs->greater_equal(rhs));
                        break;
                    case CompareCondition::Greater:
                        PUSH(lhs->greater(rhs));
                        break;
                    case CompareCondition::Not_Equal:
                        PUSH(lhs->not_equal(rhs));
                        break;
                    // is��is not�ؼ������ڱȽ�����python����ĵ�ַ�Ƿ�һ��
                    case CompareCondition::Is:
                        PUSH(packBoolean(lhs == rhs));
                        break;
                    case CompareCondition::Is_Not:
                        PUSH(packBoolean(lhs != rhs));
                        break;
                    default:
                        printf("Unknown Compare Condition Code 0x%x\n", op_arg);
                        exit(-1);
                }
                break;
            }

            // Pop_Jump_If_Falseָ�����Compare_Opָ��֮��ʹ��
            // ��ָ��ִ�е��Ǿ��Ե�ַ��ת
            case ByteCode::Pop_Jump_If_False:
                lhs = POP();
                if (lhs == Universe::PyFalse) {
                    PC = op_arg;
                }
                break;
            
            // �������ľ��Ե�ַ��ת
            case ByteCode::Jump_Absolute:
                PC = op_arg;
                break;

            // ����������Ե�ַ��ǰ��ת
            case ByteCode::Jump_Forward:
                PC += op_arg;
                break;

            // ��ʼ��Blockʵ��
            case ByteCode::Setup_Loop:
                PUSH_BLOCK(op_code, PC + op_arg, StackLength);
                break;
            
            // �����˳�����������ջ�������Ԫ��
            // ʹ��ջ�ĳ��Ȼָ��������������֮ǰ
            case ByteCode::Pop_Block: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                break;
            }

            // ѭ��ִ����;ǿ���˳�ѭ��
            case ByteCode::Break_Loop: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                PC = block._target;
                break;
            }

            // ����ջ����CodeObject���󣬾ݴ˴�������
            // ��ָ��в�������ʾ��ҪΪ�ú����󶨶��ٸ�Ĭ�ϲ���
            case ByteCode::Make_Function: {
                lhs = POP();
                PyFunction* funcObject = new PyFunction(static_cast<CodeObject*>(lhs));

                // ������Ҫ��Ĭ�ϲ����ĺ���
                int16_t defaultArgCount = op_arg;
                if (defaultArgCount > 0) {
                    PyObjectList* DefaultArgs = new PyObjectList(defaultArgCount);
                    while (defaultArgCount-- > 0) {
                        DefaultArgs->set(defaultArgCount, POP());
                    }
                    funcObject->setDefaultArgs(DefaultArgs);
                }

                // ͬ��ջ��ȫ�ֱ�����ͺ��������ȫ�ֱ�����
                funcObject->setGlobalMap(_curFrame->_globals);
                PUSH(funcObject);
                break;
            }
            
            case ByteCode::Call_Function: {
                entryIntoNewFrame(op_arg);
                break;
            }
            
            case ByteCode::Return_Value:
                // ��Ϊcallee��ջ�弴�������٣�������Ҫ������ֵ��ʱ����һ��
                _retValue = POP();
                // �˳�������calleeջ�壬�ָ�callerջ��
                exitFromCurFrame();
                break;

            // ��ջ��Ԫ�ش��浽ȫ�ֱ���
            case ByteCode::Store_Global: {
                // ջ�������Ҫд���PyObject����������Ҫ�Ȱ���������
                PyObject* sourceObject = POP();
                // �ȸ���ָ��������Ҫ���ʱ���������
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // �ٸ�������������ȫ�ֱ����Ķ���
                _curFrame->_globals->set(variableName, sourceObject);
                break;
            }
            
            // ��ָ����ȫ�ֱ���ѹ��ջ��
            case ByteCode::Load_Global: {
                // �ȸ���ָ������ҳ�Ҫ���ʱ����ı�����
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // �ٸ��ݱ�����������Python����(������None)
                PyObject* variableObject = _curFrame->_globals->get(variableName);
                // ���ҵ���ȫ�ֱ���ѹ��ջ��
                PUSH(variableObject);
                break;
            }

            // ��ջ��Ԫ�ش��浽ָ�����Ƶı��ر���
            case ByteCode::Store_Name: {
                PyObject* sourceObject = POP();
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // ����Python���ں������ڳ��ԶԱ�����ֵʱ��
                // ����ȫ����ȫ�ֱ���
                // ��������ֱ������_locals���ɣ�����ȥ���_globals��
                _curFrame->_locals->set(variableName, sourceObject);
                break;
            }
            
            // ����ָ�����Ƶı�����ջ��
            case ByteCode::Load_Name: {
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // ��ȥ���ر���������û����Ҫ�ҵı���
                PyObject* variableObject = _curFrame->_locals->get(variableName);
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // û�ҵ��Ļ���ȥȫ�ֱ�����������
                variableObject = _curFrame->_globals->get(variableName);
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // �������û�ҵ���ȥ�����ڽ���������������õ�None��
                variableObject = this->_buildins->get(variableName);
                PUSH(variableObject);
                break;
            }
            
            case ByteCode::Load_Attr: {
                // ��Ҫ�������ԵĶ���ջ
                lhs = POP();  
                // ����Ҫ��ȡ����������name
                rhs = _curFrame->_names->get(op_arg);
                // ����õ����Զ���ѹջ
                PUSH(lhs->getattr(rhs));
                break;
            }


            case ByteCode::Load_Fast:
                PUSH(_curFrame->_fastLocals->get(op_arg));
                break;

            case ByteCode::Store_Fast:
                _curFrame->_fastLocals->set(op_arg, POP());
                break;

            default:
                printf("Unknown bytecode 0x%x\n", op_code);
        }
    }
}

#define isCommonFuncKlass(k) \
    (k == FunctionKlass::getInstance() || \
    k == NativeFunctionKlass::getInstance())
#define isNativeFuncKlass(k) (k == NativeFunctionKlass::getInstance())
#define isPythonFuncKlass(k) (k == FunctionKlass::getInstance())
#define isMethod(k) (k == MethodKlass::getInstance())

// �����µ�ջ�岢����������ִ���������л�Ϊ�µ�ջ��
void Interpreter::entryIntoNewFrame(uint16_t argNumber) {
    PyObjectList* args = new PyObjectList(argNumber);
    
    // ���ص��ò�����fastLocal������
    while (argNumber-- > 0) {
        args->set(argNumber, POP());
    }
    
    // ��ջ�е���callable object
    PyObject* callableObject = POP();
    const Klass* klass = callableObject->getKlass();

    // �ж�callable object����ʵ���ͣ���ȡ��������callee function
    PyFunction* calleeFunc = nullptr;
    PyObject* owner = nullptr;
    if (isMethod(klass)) {
        PyMethod* method = static_cast<PyMethod*>(callableObject);
        calleeFunc = method->getFunc();
        owner = method->getOwner();
    }
    else if (isCommonFuncKlass(klass)) {
        calleeFunc = static_cast<PyFunction*>(callableObject);
    }
    else {
        printf("Unknown callable object!");
        exit(-1);
    }
    
    // ����һ���python��������Ҫ�����β��б��е�Ĭ�ϲ�������ʵ���б�
    if (isPythonFuncKlass(klass)) {
        PyObjectList* defaultArgs = calleeFunc->_defaultArgs;
        size_t argCnt_orignal = args->getLength();
        size_t argCnt_total = calleeFunc->funcCode->_argCount;
        size_t argCnt_default = defaultArgs->getLength();
        for (size_t i = argCnt_orignal; i < argCnt_total; ++i) {
            args->set(i, defaultArgs->get(argCnt_default - argCnt_total + i));
        }
    }

    // ���ڷ���������Ҫ������б��������������
    if (isMethod(klass)) {
        args->insert(0, owner);
    }

    // ǰ��׼��������ϣ����������ĺ�������
    klass = calleeFunc->getKlass();
    if (isNativeFuncKlass(klass)) {
        _curFrame->pushToStack(calleeFunc->callNativeFunc(args));
    }
    else if (isPythonFuncKlass(klass)) {
        // ������ջ�壬���л���������ִ��������
        FrameObject* calleeFrame = 
            new FrameObject(calleeFunc, _curFrame, args);
        _curFrame = calleeFrame;
    }
    else {
        printf("Unknown function object!");
    }
}

// �˳������ٵ�ǰջ�壬�ٰѽ�����ִ���������л�Ϊcaller��ջ��
void Interpreter::exitFromCurFrame() {
    FrameObject* callerFrame = _curFrame->_callerFrame;
    bool isRoot = _curFrame->isRootFrame();
    // ����callee��ջ��
    delete _curFrame;
    // �ָ�caller��ջ��
    _curFrame = callerFrame;
    if (!isRoot) {
        // ��callee�ķ���ֵѹ��caller������ʱջ
        PUSH(_retValue);
    }
}

