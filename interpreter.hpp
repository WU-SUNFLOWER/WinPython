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
    PyObject* _retValue;  // 该成员变量用于在切换栈桢上下文时，缓存callee的返回值
public:
    Interpreter();
    void run(CodeObject* code);

    void makeFunction(int16_t defaultArgCount, bool isClosureFunc);

    void entryIntoNewFrame(PyObject* callableObject, PyList* rawArgs, 
        uint8_t argNumber_pos, uint8_t argNumber_kw);

    void exitFromCurFrame();
};

#endif