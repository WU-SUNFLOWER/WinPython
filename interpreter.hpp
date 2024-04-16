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

    void makeFunction(int16_t defaultArgCount, bool isClosureFunc);

    void entryIntoNewFrame(PyObject* callableObject, PyList* rawArgs, 
        uint8_t argNumber_pos, uint8_t argNumber_kw);

    void exitFromCurFrame();
};

#endif