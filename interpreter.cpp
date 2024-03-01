#include "interpreter.hpp"

Interpreter::~Interpreter() {

}

void Interpreter::run(CodeObject* code) {
    // 指向当前正在执行指令的指针
    size_t pc = 0;
    // 从CodeObject对象中取出二进制指令流
    PyString instructions = *(code->_byteCodes);
    // 指令流总长度（字节）
    size_t instrLength = instructions.getLength();
    // 最大栈空间
    int64_t stackSize = code->_stackSize;
    // 初始化运行时栈
    runtimeStack = new ArrayList<PyObject*>(stackSize);
    // 代码中的字面量
    consts = code->_consts;

#define POP() (runtimeStack->pop())
#define PUSH(x) (runtimeStack->push(x))

    while (pc < instrLength) {
        uint8_t op_code = instructions[pc++];
        bool hasArugment = op_code >= ByteCode::Have_Argument;
        uint16_t op_arg = -1;

        // 如果指令含参，则一并取出，取的时候注意用小端序
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
                rhs = POP();  // 右操作数
                lhs = POP();  // 左操作数
                PUSH(lhs->add(rhs));
                break;

            case ByteCode::Compare_Op: {
                rhs = POP();  // 右操作数
                lhs = POP();  // 左操作数
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

            // Pop_Jump_If_False指令跟在Compare_Op指令之后使用
            // 该指令执行的是绝对地址跳转
            case ByteCode::Pop_Jump_If_False:
                lhs = POP();
                if (lhs == Universe::PyFalse) {
                    pc = op_arg;
                }
                break;
            
            // 无条件的相对地址向前跳转
            case ByteCode::Jump_Forward:
                pc += op_arg;
                break;

            // 弹出并返回栈顶元素
            case ByteCode::Return_Value:
                POP();
                break;

            default:
                printf("Unknown bytecode 0x%x", op_code);
        }
    }
}