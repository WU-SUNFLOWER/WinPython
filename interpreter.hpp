#ifndef Interpreter_Hpp
#define Interpreter_Hpp

#include "CodeObject.hpp"
#include "PyInteger.hpp"
#include "PyFunction.hpp"
#include "Universe.hpp"
#include "bytecode.hpp"
#include "Map.hpp"

class Interpreter {
private:
    PyObjectMap* _buildins;
    FrameObject* _curFrame;
    PyObject* _retValue;  // �ó�Ա�����������л�ջ��������ʱ������callee�ķ���ֵ
public:
    Interpreter();
    void run(CodeObject* code);

    void entryIntoNewFrame(uint16_t argNumber);

    void lauchCallable(PyObject* callable, PyObjectList* args);

    void exitFromCurFrame();
};

#endif