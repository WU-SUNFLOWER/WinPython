#include "interpreter.hpp"

Interpreter::~Interpreter() {

}

void Interpreter::run(CodeObject* code) {
    // ָ��ǰ����ִ��ָ���ָ��
    size_t pc = 0;
    // ��CodeObject������ȡ��������ָ����
    PyString instructions = *(code->_byteCodes);
    // ָ�����ܳ��ȣ��ֽڣ�
    size_t instrLength = instructions.getLength();
    // ���ջ�ռ�
    int64_t stackSize = code->_stackSize;
    // ��ʼ������ʱջ
    runtimeStack = new ArrayList<PyObject*>(stackSize);
    // �����е�������
    consts = code->_consts;

#define POP() (runtimeStack->pop())
#define PUSH(x) (runtimeStack->push(x))

    while (pc < instrLength) {
        uint8_t op_code = instructions[pc++];
        bool hasArugment = op_code >= ByteCode::Have_Argument;
        uint16_t op_arg = -1;

        // ���ָ��Σ���һ��ȡ����ȡ��ʱ��ע����С����
        if (hasArugment) {
            uint16_t little = instructions[pc++];
            op_arg = (instructions[pc++] << 8) | little;
        }
        
        PyObject* rhs, * lhs;

        switch (op_code) {
            case ByteCode::Load_Const:
                PUSH(consts->get(op_arg));
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
                    pc = op_arg;
                }
                break;
            
            // ����������Ե�ַ��ǰ��ת
            case ByteCode::Jump_Forward:
                pc += op_arg;
                break;

            // ����������ջ��Ԫ��
            case ByteCode::Return_Value:
                POP();
                break;

            default:
                printf("Unknown bytecode 0x%x", op_code);
        }
    }
}