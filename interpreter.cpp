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

        // 如果指令含参，则一并取出，取的时候注意用小端序
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
                // 因为可能有多个多个元素要存入字典，
                // 所以字典对象暂时不能从栈上弹出
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
                rhs = POP();  // 右操作数
                lhs = POP();  // 左操作数
                PUSH(lhs->mul(rhs));
                break;

            case ByteCode::Binary_Add:
                rhs = POP();  // 右操作数
                lhs = POP();  // 左操作数
                PUSH(lhs->add(rhs));
                break;

            case ByteCode::Binary_Subtract:
                rhs = POP();  // 右操作数
                lhs = POP();  // 左操作数
                PUSH(lhs->sub(rhs));
                break;

            case ByteCode::Binary_Subscr:
                rhs = POP();  // 右操作数（在此处为要取的下标）
                lhs = POP();  // 左操作数（在此处为被取下标的元素）
                PUSH(lhs->subscr(rhs));
                break;

            case ByteCode::Store_Subscr: {
                rhs = POP();  // 右操作数（在此处为要取的下标）
                lhs = POP();  // 左操作数（在此处为被取下标的元素）
                PyObject* newObject = POP();
                lhs->store_subscr(rhs, newObject);
                break;
            }

            case ByteCode::Delete_Subscr: {
                rhs = POP();  // 右操作数（在此处为要取的下标）
                lhs = POP();  // 左操作数（在此处为被取下标的元素）
                lhs->delete_subscr(rhs);
                break;
            }

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
            // 该指令含有参数，表示需要为该函数绑定多少个默认参数
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
                // 因为callee的栈桢即将被销毁，所以需要将返回值临时缓存一下
                _retValue = POP();
                // 退出并销毁callee栈桢，恢复caller栈桢
                exitFromCurFrame();
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
                if (variableObject != Universe::PyNone) {
                    PUSH(variableObject);
                    break;
                }
                // 如果还是没找到，去看看内建变量表（可能最后会得到None）
                variableObject = this->_buildins->get(variableName);
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
            
            case ByteCode::Load_Attr: {
                // 将要查找属性的对象弹栈
                lhs = POP();  
                // 查找要获取的属性名的name
                rhs = _curFrame->_names->get(op_arg);
                // 将查得的属性对象压栈
                PUSH(lhs->getattr(rhs));
                break;
            }


            case ByteCode::Load_Fast:
                PUSH(_curFrame->_fastLocals->get(op_arg));
                break;

            case ByteCode::Store_Fast:
                _curFrame->_fastLocals->set(op_arg, POP());
                break;

            // 弹出栈顶元素，将其挂载到运行时的cells表中
            /*
                在Python的闭包机制中，只有outer function才有可能调用Store_Deref
                如果在inner function中尝试对同名变量赋值，虚拟机会为inner function
                创建新的local variable而不是修改outer function中的同名变量
            */
            case ByteCode::Store_Deref:
                _curFrame->_cells->set(op_arg, POP());
                break;
            
            // 获取cell variable并解引用，再加载到栈顶
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


            // 将cells表中的元素转换为PyCell并压栈
            // 该指令与Make_Closure指令配合使用，用于构建闭包函数
            case ByteCode::Load_Closure: {
                PyObject* cellObject = _curFrame->_cells->get(op_arg);
                /* 
                    如果在cells列表里没找到，也有可能是inner function恰好引用
                    outer function的argument。所以还要再去尝试找找。
                */
                if (!cellObject) {
                    PyObject* cellName = 
                        _curFrame->_codeObject->_cellVars->get(op_arg);
                    size_t i = _curFrame->_varNames->index(cellName);
                    cellObject = _curFrame->_fastLocals->get(i);
                    // 找到了之后别忘了把cellObject挂载到栈桢的_cells上去
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
            
            // 从栈上弹出要获取迭代器的元素，再把获取到的迭代器压回栈上
            case ByteCode::Get_Iter: {
                lhs = POP();
                PUSH(lhs->getIter());
                break;
            }
            
            case ByteCode::For_Iter: {
                // 获取迭代器对象（由于迭代器要重复使用，所以不能从栈上弹出）
                PyObject* iter = TOP();
                // 调用迭代器对象的next方法
                PyObject* ret = iter->next();
                // 如果返回值是None，则说明迭代结束，执行跳转
                // 注意这里必须用TOP不能用POP，
                // 因为迭代结束前留在栈上的返回值可能会被后续指令使用
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
                // Python中解包值接收变量数必须和待解包列表的长度一致
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
                // 从后往前解包，确保列表靠左元素可以位于栈顶，被后续指令先取到
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

    // 如果需要创建的函数对象为闭包函数，需要绑定cells列表
    if (isClosure) {
        PyObject* temp = POP();
        funcObject->setFreevars(static_cast<PyList*>(temp));
    }

    // 处理需要绑定默认参数的函数
    if (defaultArgCount > 0) {
        PyList* DefaultArgs = new PyList(defaultArgCount);
        while (defaultArgCount-- > 0) {
            DefaultArgs->set(defaultArgCount, POP());
        }
        funcObject->setDefaultArgs(DefaultArgs);
    }

    // 同步栈桢全局变量表和函数对象的全局变量表
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
    
    // 加载调用参数到fastLocal数组中
    while (argNumber-- > 0) {
        args->set(argNumber, POP());
    }
    
    // 从栈中弹出callable object
    PyObject* callableObject = POP();
    const Klass* klass = callableObject->getKlass();
    if (!isMethod(klass) && !isCommonFuncKlass(klass)) {
        printf("Unknown callable object!");
        exit(-1);
    }

    // 判断callable object的真实类型，提取出真正的callee function
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

    // 对于一般的python函数，需要根据形参列表中的默认参数更新实参列表
    PyList* defaultArgs = calleeFunc->_defaultArgs;
    if (isPythonFuncKlass(klass) && defaultArgs) {
        size_t argCnt_orignal = args->getLength();
        size_t argCnt_total = calleeFunc->funcCode->_argCount;
        size_t argCnt_default = defaultArgs->getLength();
        for (size_t i = argCnt_orignal; i < argCnt_total; ++i) {
            args->set(i, defaultArgs->get(argCnt_default - argCnt_total + i));
        }
    }

    // 对于方法，还需要向参数列表中添加宿主对象
    if (isMethod(klass)) {
        args->insert(0, owner);
    }

    // 前期准备工作完毕，发起真正的函数调用
    klass = calleeFunc->getKlass();
    if (isNativeFuncKlass(klass)) {
        _curFrame->pushToStack(calleeFunc->callNativeFunc(args));
    }
    else if (isPythonFuncKlass(klass)) {
        // 创建新栈桢
        FrameObject* calleeFrame = 
            new FrameObject(calleeFunc, _curFrame, args);
        /* 
           将与callee绑定的cells（即callee函数运行时
           需要依赖的freevars），装载到新的栈桢上去。
           这样当callee运行时，就可以通过Load_Closure指令
           访问这些freevars了。
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
        // 别忘了最后一步，切换解释器的执行上下文
        _curFrame = calleeFrame;
    }
    else {
        printf("Unknown function object!");
    }
}

// 退出并销毁当前栈桢，再把解释器执行上下文切换为caller的栈桢
void Interpreter::exitFromCurFrame() {
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

