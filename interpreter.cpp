#include "interpreter.hpp"
#include "FrameObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include <cstdlib>
#include "Block.hpp"

Interpreter::Interpreter() {
    _curFrame = nullptr;
    _retValue = nullptr;
    _buildins = new PyObjectMap();
    _buildins->set(new PyString("True"), Universe::PyTrue);
    _buildins->set(new PyString("False"), Universe::PyFalse);
    _buildins->set(Universe::PyNone, Universe::PyNone);
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
            case ByteCode::Make_Function: {
                lhs = POP();
                PyFunction* funcObject = new PyFunction(static_cast<CodeObject*>(lhs));
                // ͬ��ջ��ȫ�ֱ�����ͺ��������ȫ�ֱ�����
                funcObject->setGlobalMap(_curFrame->_globals);
                PUSH(funcObject);
                break;
            }
            
            case ByteCode::Call_Function: {
                entryNewFrame(op_arg);
                break;
            }
            
            case ByteCode::Return_Value:
                // ��Ϊcallee��ջ�弴�������٣�������Ҫ������ֵ��ʱ����һ��
                _retValue = POP();
                // �˳�������calleeջ�壬�ָ�callerջ��
                exitCurFrame();
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

// �����µ�ջ�岢����������ִ���������л�Ϊ�µ�ջ��
void Interpreter::entryNewFrame(uint16_t argNumber) {
    PyObjectList* fastLocalAr = new PyObjectList(argNumber);
    // ���ص��ò�����fastLocal������
    while (argNumber-- > 0) {
        fastLocalAr->set(argNumber, POP());
    }
    PyFunction* callee = static_cast<PyFunction*>(POP());
    FrameObject* calleeFrame = new FrameObject(callee, _curFrame, fastLocalAr);
    _curFrame = calleeFrame;
}

// �˳������ٵ�ǰջ�壬�ٰѽ�����ִ���������л�Ϊcaller��ջ��
void Interpreter::exitCurFrame() {
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

