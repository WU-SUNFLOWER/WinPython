#include "FrameObject.hpp"
#include "interpreter.hpp"

FrameObject::FrameObject(CodeObject* codeObject) {
    pc = 0;
    _codeObject = codeObject;
    _consts = codeObject->_consts;
    _names = codeObject->_names;
    _varNames = codeObject->_varNames;
    _byteCodes = codeObject->_byteCodes;
    PUSH_TEMP(_codeObject);
    PUSH_TEMP(_consts);
    PUSH_TEMP(_names);
    PUSH_TEMP(_varNames);
    PUSH_TEMP(_byteCodes);

    // 初始化一张空的映射表用于储存本地变量
    _locals = new PyDict();
    PUSH_TEMP(_locals);
    
    // 对于入口函数<module>来说，它外面不可能再有全局变量
    // 因此该函数中的变量查询，到它的本地变量为止
    _globals = _locals;
    _fastLocals = nullptr;

    size_t cellsLength = 
        _codeObject->_cellVars->getLength() + _codeObject->_freeVars->getLength();
    _cells = cellsLength > 0 ? new PyList(cellsLength) : nullptr;
    PUSH_TEMP(_cells);

    if (_cells) {
        for (size_t i = 0; i < cellsLength; ++i) {
            _cells->set(i, nullptr);
        }
    }

    _stack = new PyList(_codeObject->_stackSize);
    PUSH_TEMP(_stack);

    _blockStack = new ArrayList<Block>({});
    PUSH_TEMP(_blockStack);

    _callerFrame = nullptr;

    _isEntryFrame = false;

    POP_TEMP(9);

}

/*
FrameObject::FrameObject(PyFunction* callee, FrameObject* callerFrame, 
    bool isEntryFrame, PyList* args) : 

    FrameObject(callee->funcCode) {
    // 将函数调用所产生栈桢的全局变量表，同步为函数绑定的变量表
    _globals = callee->_globals;
    _callerFrame = callerFrame;
    _fastLocals = args;
    _isEntryFrame = isEntryFrame;
}
*/

FrameObject* FrameObject::allocate(PyFunction* callee, FrameObject* callerFrame,
    bool isEntryFrame, PyList* args
) {
    PUSH_TEMP(callee);
    PUSH_TEMP(callerFrame);
    PUSH_TEMP(args);
    FrameObject* frame = new FrameObject(callee->funcCode);
    frame->_globals = callee->_globals;
    frame->_callerFrame = callerFrame;
    frame->_fastLocals = args;
    frame->_isEntryFrame = isEntryFrame;
    POP_TEMP(3);
    return frame;
}

uint8_t FrameObject::getOpCode() {
    PyString& instructions = *(_byteCodes);
    return instructions[pc++];
}

uint16_t FrameObject::getOpArgument() {
    PyString& instructions = *(_byteCodes);
    uint16_t little = instructions[pc++];
    // 取指令参数的时候记得用小端序取出
    return (instructions[pc++] << 8) | little;
}

bool FrameObject::hasMoreCode() {
    return pc < _byteCodes->getLength();
}

void FrameObject::pushToStack(PyObject* object)
{
    _stack->append(object);
}

PyObject* FrameObject::popFromStack()
{
    return _stack->pop();
}

PyObject* FrameObject::getTopInStack() {
    return _stack->top();
}

bool FrameObject::isRootFrame() const
{
    return _callerFrame == nullptr;
}

bool FrameObject::isEntryFrame() const
{
    return _isEntryFrame;
}

void FrameObject::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(& _consts));
    closure->do_oop(reinterpret_cast<PyObject**>(&_names));

    closure->do_oop(reinterpret_cast<PyObject**>(&_globals));
    closure->do_oop(reinterpret_cast<PyObject**>(&_locals));
    closure->do_oop(reinterpret_cast<PyObject**>(&_fastLocals));
    closure->do_oop(reinterpret_cast<PyObject**>(&_cells));

    closure->do_oop(reinterpret_cast<PyObject**>(&_stack));

    closure->do_oop(reinterpret_cast<PyObject**>(&_byteCodes));

    closure->do_array_list(&_blockStack);

    if (_callerFrame) _callerFrame->oops_do(closure);
}