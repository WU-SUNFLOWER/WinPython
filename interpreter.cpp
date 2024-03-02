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

        // 如果指令含参，则一并取出，取的时候注意用小端序
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
                    // is和is not关键字用于比较两个python对象的地址是否一致
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

            // Pop_Jump_If_False指令跟在Compare_Op指令之后使用
            // 该指令执行的是绝对地址跳转
            case ByteCode::Pop_Jump_If_False:
                lhs = POP();
                if (lhs == Universe::PyFalse) {
                    PC = op_arg;
                }
                break;
            
            // 无条件的绝对地址跳转
            case ByteCode::Jump_Absolute:
                PC = op_arg;
                break;

            // 无条件的相对地址向前跳转
            case ByteCode::Jump_Forward:
                PC += op_arg;
                break;

            // 初始化Block实例
            case ByteCode::Setup_Loop:
                PUSH_BLOCK(op_code, PC + op_arg, StackLength);
                break;
            
            // 程序退出代码块后，清理栈顶多余的元素
            // 使得栈的长度恢复到程序进入代码块之前
            case ByteCode::Pop_Block: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                break;
            }

            // 循环执行中途强制退出循环
            case ByteCode::Break_Loop: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                PC = block._target;
                break;
            }

            // 弹出栈顶的CodeObject对象，据此创建函数
            case ByteCode::Make_Function: {
                lhs = POP();
                PyFunction* funcObject = new PyFunction(static_cast<CodeObject*>(lhs));
                // 同步栈桢全局变量表和函数对象的全局变量表
                funcObject->setGlobalMap(_curFrame->_globals);
                PUSH(funcObject);
                break;
            }
            
            case ByteCode::Call_Function: {
                entryNewFrame(op_arg);
                break;
            }
            
            case ByteCode::Return_Value:
                // 因为callee的栈桢即将被销毁，所以需要将返回值临时缓存一下
                _retValue = POP();
                // 退出并销毁callee栈桢，恢复caller栈桢
                exitCurFrame();
                break;

            // 将栈顶元素储存到全局变量
            case ByteCode::Store_Global: {
                // 栈顶存放着要写入的PyObject，所以我们要先把它弹出来
                PyObject* sourceObject = POP();
                // 先根据指令参数查出要访问变量的名称
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // 再根据名称索引到全局变量的对象
                _curFrame->_globals->set(variableName, sourceObject);
                break;
            }
            
            // 将指定的全局变量压入栈顶
            case ByteCode::Load_Global: {
                // 先根据指令参数找出要访问变量的变量名
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // 再根据变量名索引到Python对象(可能是None)
                PyObject* variableObject = _curFrame->_globals->get(variableName);
                // 将找到的全局变量压入栈顶
                PUSH(variableObject);
                break;
            }

            // 将栈顶元素储存到指定名称的本地变量
            case ByteCode::Store_Name: {
                PyObject* sourceObject = POP();
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // 由于Python中在函数体内尝试对变量赋值时，
                // 会完全屏蔽全局变量
                // 所以这里直接设置_locals表即可，无需去检查_globals表
                _curFrame->_locals->set(variableName, sourceObject);
                break;
            }
            
            // 加载指定名称的变量到栈顶
            case ByteCode::Load_Name: {
                PyObject* variableName = _curFrame->_names->get(op_arg);
                // 先去本地变量表看看有没有想要找的变量
                PyObject* variableObject = _curFrame->_locals->get(variableName);
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // 没找到的话再去全局变量表里找找
                variableObject = _curFrame->_globals->get(variableName);
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // 如果还是没找到，去看看内建变量表（可能最后会得到None）
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

// 创建新的栈桢并将解释器的执行上下文切换为新的栈桢
void Interpreter::entryNewFrame(uint16_t argNumber) {
    PyObjectList* fastLocalAr = new PyObjectList(argNumber);
    // 加载调用参数到fastLocal数组中
    while (argNumber-- > 0) {
        fastLocalAr->set(argNumber, POP());
    }
    PyFunction* callee = static_cast<PyFunction*>(POP());
    FrameObject* calleeFrame = new FrameObject(callee, _curFrame, fastLocalAr);
    _curFrame = calleeFrame;
}

// 退出并销毁当前栈桢，再把解释器执行上下文切换为caller的栈桢
void Interpreter::exitCurFrame() {
    FrameObject* callerFrame = _curFrame->_callerFrame;
    bool isRoot = _curFrame->isRootFrame();
    // 销毁callee的栈桢
    delete _curFrame;
    // 恢复caller的栈桢
    _curFrame = callerFrame;
    if (!isRoot) {
        // 将callee的返回值压入caller的运行时栈
        PUSH(_retValue);
    }
}

