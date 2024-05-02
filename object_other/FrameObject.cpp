#include "FrameObject.hpp"

FrameObject::FrameObject(CodeObject* codeObject) {
    pc = 0;
    _codeObject = codeObject;
    _consts = codeObject->_consts;
    _names = codeObject->_names;
    _varNames = codeObject->_varNames;
    _byteCodes = codeObject->_byteCodes;
    
    // ��ʼ��һ�ſյ�ӳ������ڴ��汾�ر���
    _locals = new PyDict();
    // ������ں���<module>��˵�������治��������ȫ�ֱ���
    // ��˸ú����еı�����ѯ�������ı��ر���Ϊֹ
    _globals = _locals;
    _fastLocals = nullptr;

    size_t cellsLength = 
        codeObject->_cellVars->getLength() + codeObject->_freeVars->getLength();
    _cells = cellsLength > 0 ? new PyList(cellsLength) : nullptr;
    if (_cells) {
        for (size_t i = 0; i < cellsLength; ++i) {
            _cells->set(i, nullptr);
        }
    }

    _stack = new PyList(codeObject->_stackSize);
    _blockStack = new ArrayList<Block>({});

    _callerFrame = nullptr;

    _isEntryFrame = false;
}

FrameObject::FrameObject(PyFunction* callee, FrameObject* callerFrame, 
    bool isEntryFrame, PyList* args) : FrameObject(callee->funcCode) {
    // ����������������ջ���ȫ�ֱ�����ͬ��Ϊ�����󶨵ı�����
    _globals = callee->_globals;
    _callerFrame = callerFrame;
    _fastLocals = args;
    _isEntryFrame = isEntryFrame;
}

uint8_t FrameObject::getOpCode() {
    PyString& instructions = *(_byteCodes);
    return instructions[pc++];
}

uint16_t FrameObject::getOpArgument() {
    PyString& instructions = *(_byteCodes);
    uint16_t little = instructions[pc++];
    // ȡָ�������ʱ��ǵ���С����ȡ��
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