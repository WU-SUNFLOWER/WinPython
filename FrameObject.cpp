#include "FrameObject.hpp"

FrameObject::FrameObject(CodeObject* codeObject) {
    pc = 0;
    _codeObject = codeObject;
    _consts = codeObject->_consts;
    _names = codeObject->_names;
    _byteCodes = codeObject->_byteCodes;
    
    // ��ʼ��һ�ſյ�ӳ������ڴ��汾�ر���
    _locals = new PyObjectMap();
    // ������ں���<module>��˵�������治��������ȫ�ֱ���
    // ��˸ú����еı�����ѯ�������ı��ر���Ϊֹ
    _globals = _locals;
    _fastLocals = nullptr;

    _stack = new PyObjectList(codeObject->_stackSize);
    _blockStack = new ArrayList<Block>();

    _callerFrame = nullptr;
}

FrameObject::FrameObject(PyFunction* callee, FrameObject* callerFrame, 
    PyObjectList* args) : FrameObject(callee->funcCode) {
    // ����������������ջ���ȫ�ֱ�����ͬ��Ϊ�����󶨵ı�����
    _globals = callee->_globals;
    _callerFrame = callerFrame;
    _fastLocals = args;
}

FrameObject::~FrameObject() {
    delete _locals;
    delete _stack;
    delete _fastLocals;
    delete _blockStack;
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
    _stack->push(object);
}

PyObject* FrameObject::popFromStack()
{
    return _stack->pop();
}

bool FrameObject::isRootFrame() const
{
    return _callerFrame == nullptr;
}
