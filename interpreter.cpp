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
#include "CellKlass.hpp"
#include "PyCell.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "ListKlass.hpp"
#include "StringTable.hpp"
#include "DictKlass.hpp"

Interpreter::Interpreter() {
    _curFrame = nullptr;
    _retValue = nullptr;
    _buildins = new PyObjectMap();

    _buildins->set(new PyString("True"), Universe::PyTrue);
    _buildins->set(new PyString("False"), Universe::PyFalse);
    _buildins->set(Universe::PyNone, Universe::PyNone);
    
    _buildins->set(new PyString("len"), PackNativeFunc(NativeFunction::len));

    DictKlass::getInstance()->initialize();
    ListKlass::getInstance()->initialize();
    StringKlass::getInstance()->initialize();

}

#define POP() (_curFrame->popFromStack())
#define TOP() (_curFrame->getTopInStack())
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

            case ByteCode::Build_List:{
                PyList* list = new PyList();
                while (op_arg-- > 0) {
                    PyObject* temp = POP();
                    list->set(op_arg, temp);
                }
                PUSH(list);
                break;
            }
            
            case ByteCode::Build_Map: {
                PUSH(new PyDict());
                break;
            }

            case ByteCode::Store_Map: {
                PyObject* key = POP();
                PyObject* value = POP();
                // ��Ϊ�����ж�����Ԫ��Ҫ�����ֵ䣬
                // �����ֵ������ʱ���ܴ�ջ�ϵ���
                PyDict* dict = static_cast<PyDict*>(TOP());
                dict->set(key, value);
                break;
            }

            case ByteCode::Load_Const: {
                PyObject* temp = Consts->get(op_arg);
                PUSH(temp);
                break;
            }

            case ByteCode::Print_Item:
                lhs = POP();
                lhs ->print();
                break;

            case ByteCode::Print_NewLine:
                putchar('\n');
                break;

            case ByteCode::Binary_Multiply:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                PUSH(lhs->mul(rhs));
                break;

            case ByteCode::Binary_Add:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                PUSH(lhs->add(rhs));
                break;

            case ByteCode::Binary_Subtract:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                PUSH(lhs->sub(rhs));
                break;

            case ByteCode::Binary_Subscr:
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                PUSH(lhs->subscr(rhs));
                break;

            case ByteCode::Store_Subscr: {
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                PyObject* newObject = POP();
                lhs->store_subscr(rhs, newObject);
                break;
            }

            case ByteCode::Delete_Subscr: {
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                lhs->delete_subscr(rhs);
                break;
            }

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
                    case CompareCondition::In:
                        PUSH(rhs->has(lhs));
                        break;
                    case CompareCondition::Not_In:
                        PUSH(
                            rhs->has(lhs) == Universe::PyTrue ? 
                            Universe::PyFalse : Universe::PyTrue
                        );
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
                makeFunction(op_arg, false);
                break;
            }
            
            case ByteCode::Make_Closure: {
                makeFunction(op_arg, true);
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
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // �������û�ҵ���ȥ�����ڽ���������������õ�None��
                variableObject = this->_buildins->get(variableName);
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

            // ����ջ��Ԫ�أ�������ص�����ʱ��cells����
            /*
                ��Python�ıհ������У�ֻ��outer function���п��ܵ���Store_Deref
                �����inner function�г��Զ�ͬ��������ֵ���������Ϊinner function
                �����µ�local variable�������޸�outer function�е�ͬ������
            */
            case ByteCode::Store_Deref:
                _curFrame->_cells->set(op_arg, POP());
                break;
            
            // ��ȡcell variable�������ã��ټ��ص�ջ��
            case ByteCode::Load_Deref: {
                PyObject* object = _curFrame->_cells->get(op_arg);
                if (object->getKlass() == CellKlass::getInstance()) {
                    PyCell* cell = static_cast<PyCell*>(object);
                    PyObject* temp = cell->getObject();
                    PUSH(temp);
                }
                else {
                    PUSH(object);
                }
                break;
            }


            // ��cells���е�Ԫ��ת��ΪPyCell��ѹջ
            // ��ָ����Make_Closureָ�����ʹ�ã����ڹ����հ�����
            case ByteCode::Load_Closure: {
                PyObject* cellObject = _curFrame->_cells->get(op_arg);
                /* 
                    �����cells�б���û�ҵ���Ҳ�п�����inner functionǡ������
                    outer function��argument�����Ի�Ҫ��ȥ�������ҡ�
                */
                if (!cellObject) {
                    PyObject* cellName = 
                        _curFrame->_codeObject->_cellVars->get(op_arg);
                    size_t i = _curFrame->_varNames->index(cellName);
                    cellObject = _curFrame->_fastLocals->get(i);
                    // �ҵ���֮������˰�cellObject���ص�ջ���_cells��ȥ
                    _curFrame->_cells->set(op_arg, cellObject);
                }
                assert(cellObject != nullptr);
                if (cellObject->getKlass() != CellKlass::getInstance()) {
                    cellObject = new PyCell(_curFrame->_cells, op_arg);
                }
                PyObject* temp = static_cast<PyCell*>(cellObject)->getObject();
                PUSH(cellObject);
                break;
            }
            
            // ��ջ�ϵ���Ҫ��ȡ��������Ԫ�أ��ٰѻ�ȡ���ĵ�����ѹ��ջ��
            case ByteCode::Get_Iter: {
                lhs = POP();
                PUSH(lhs->getIter());
                break;
            }
            
            case ByteCode::For_Iter: {
                // ��ȡ�������������ڵ�����Ҫ�ظ�ʹ�ã����Բ��ܴ�ջ�ϵ�����
                PyObject* iter = TOP();
                // ���õ����������next����
                PyObject* ret = iter->next();
                // �������ֵ��None����˵������������ִ����ת
                // ע�����������TOP������POP��
                // ��Ϊ��������ǰ����ջ�ϵķ���ֵ���ܻᱻ����ָ��ʹ��
                if (ret == Universe::PyNone) {
                    PC += op_arg;
                }
                else {
                    PUSH(ret);
                }
                break;
            }

            case ByteCode::Unpack_Sequence: {
                PyList* list = static_cast<PyList*>(POP());
                // Python�н��ֵ���ձ���������ʹ�����б�ĳ���һ��
                size_t length = list->getLength();
                if (op_arg < length) {
                    printf("too many values to unpack (expected %lld)", length);
                    exit(-1);
                }
                else if (op_arg > length) {
                    printf("not enough values to unpack (expected %d, got %lld)", 
                        op_arg, length);
                    exit(-1);
                }
                // �Ӻ���ǰ�����ȷ���б���Ԫ�ؿ���λ��ջ����������ָ����ȡ��
                while (op_arg-- > 0) {
                    PUSH(list->get(op_arg));
                }
                break;
            }
                

            default:
                printf("Unknown bytecode 0x%x\n", op_code);
        }
    }
}

void Interpreter::makeFunction(int16_t defaultArgCount, bool isClosure) {
    PyFunction* funcObject = new PyFunction(static_cast<CodeObject*>(POP()));

    // �����Ҫ�����ĺ�������Ϊ�հ���������Ҫ��cells�б�
    if (isClosure) {
        PyObject* temp = POP();
        funcObject->setFreevars(static_cast<PyList*>(temp));
    }

    // ������Ҫ��Ĭ�ϲ����ĺ���
    if (defaultArgCount > 0) {
        PyList* DefaultArgs = new PyList(defaultArgCount);
        while (defaultArgCount-- > 0) {
            DefaultArgs->set(defaultArgCount, POP());
        }
        funcObject->setDefaultArgs(DefaultArgs);
    }

    // ͬ��ջ��ȫ�ֱ�����ͺ��������ȫ�ֱ�����
    funcObject->setGlobalMap(_curFrame->_globals);
    PUSH(funcObject);
}

#define isCommonFuncKlass(k) \
    (k == FunctionKlass::getInstance() || \
    k == NativeFunctionKlass::getInstance())
#define isNativeFuncKlass(k) (k == NativeFunctionKlass::getInstance())
#define isPythonFuncKlass(k) (k == FunctionKlass::getInstance())
#define isMethod(k) (k == MethodKlass::getInstance())

void Interpreter::entryIntoNewFrame(uint16_t argNumber) {
    PyList* args = new PyList(argNumber);
    
    // ���ص��ò�����fastLocal������
    while (argNumber-- > 0) {
        args->set(argNumber, POP());
    }
    
    // ��ջ�е���callable object
    PyObject* callableObject = POP();
    const Klass* klass = callableObject->getKlass();
    if (!isMethod(klass) && !isCommonFuncKlass(klass)) {
        printf("Unknown callable object!");
        exit(-1);
    }

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
    
    if (!calleeFunc) {
        printf("Try to call a function with null pointer");
        exit(-1);
    }

    // ����һ���python��������Ҫ�����β��б��е�Ĭ�ϲ�������ʵ���б�
    PyList* defaultArgs = calleeFunc->_defaultArgs;
    if (isPythonFuncKlass(klass) && defaultArgs) {
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
        // ������ջ��
        FrameObject* calleeFrame = 
            new FrameObject(calleeFunc, _curFrame, args);
        /* 
           ����callee�󶨵�cells����callee��������ʱ
           ��Ҫ������freevars����װ�ص��µ�ջ����ȥ��
           ������callee����ʱ���Ϳ���ͨ��Load_Closureָ��
           ������Щfreevars�ˡ�
        */
        PyList* freevarsList = calleeFunc->_freevars;
        if (freevarsList) {
            size_t freevar_len = freevarsList->getLength();
            size_t cells_len = calleeFunc->funcCode->_cellVars->getLength();
            for (size_t i = 0; i < freevar_len; ++i) {
                PyCell* temp = static_cast<PyCell*>(freevarsList->get(i));
                PyObject* temp1 = temp->getObject();
                calleeFrame->_cells->set(cells_len + i, temp);
            }
        }
        // ���������һ�����л���������ִ��������
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

