#include "FrameObject.hpp"
#include "interpreter.hpp"
#include "StringTable.hpp"
#include "Handle.hpp"
#include "nativeFunctions.hpp"

FrameObject::FrameObject(Handle<CodeObject*> codeObject) {

    this->pc = 0;
    this->_codeObject = codeObject;
    this->_consts = codeObject->_consts;
    this->_names = codeObject->_names;
    this->_varNames = codeObject->_varNames;
    this->_byteCodes = codeObject->_byteCodes;

    Handle<PyDict*> locals = PyDict::createDict();

    size_t cellsLength =
        _codeObject->_cellVars->getLength() + _codeObject->_freeVars->getLength();
    Handle<PyList*> cells = 
        cellsLength > 0 ? PyList::createList(cellsLength) : nullptr;
    if (cells) {
        for (size_t i = 0; i < cellsLength; ++i) {
            cells->set(i, nullptr);
        }
    }
    
    Handle<PyList*> stack = PyList::createList(_codeObject->_stackSize);

    // 初始化一张空的映射表用于储存本地变量
    this->_locals = locals;
    this->_cells = cells;
    // 对于入口函数<module>来说，它外面不可能再有全局变量
    // 因此该函数中的变量查询，到它的本地变量为止
    this->_globals = locals;
    this->_stack = stack;

    this->_fastLocals = nullptr;
    this->_blockStack = new Stack<Block>(10);
    this->_callerFrame = nullptr;
    this->_isEntryFrame = false;

}

FrameObject::~FrameObject() {
    delete _blockStack;
}

FrameObject* FrameObject::allocate(
    Handle<PyFunction*> callee, Handle<FrameObject*> callerFrame,
    bool isEntryFrame, Handle<PyList*> args
) {
    //START_COUNT_TEMP_OBJECTS;
    //PUSH_TEMP(callee);
    //PUSH_TEMP(callerFrame);
    //PUSH_TEMP(args);
    Handle<FrameObject*> frame = new FrameObject(callee->funcCode);
    //NativeFunction::sysgc(nullptr);
    frame->_globals = callee->_globals;
    frame->_callerFrame = callerFrame;
    frame->_fastLocals = args;
    frame->_isEntryFrame = isEntryFrame;
    if (callee->getOwnerClass()) {
        frame->_globals->set(StringTable::str_class, 
            reinterpret_cast<PyObject*>(callee->getOwnerClass()));
    }
    //END_COUNT_TEMP_OBJECTS;
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

    if (_callerFrame) _callerFrame->oops_do(closure);
}