#ifndef Interpreter_Hpp
#define Interpreter_Hpp

#include "Stack.hpp"
#include "CodeObject.hpp"
#include "PyInteger.hpp"
#include "PyFunction.hpp"
#include "Universe.hpp"
#include "bytecode.hpp"
#include "Map.hpp"
#include "PyDict.hpp"
#include "OopClosure.hpp"

class Interpreter {
    enum Status {
        IS_OK,
        IS_BREAK,
        IS_CONTINUE,
        IS_EXCEPTION,
        IS_RETURN,
        IS_YIELD,
    };

private:
    Interpreter();
    static Interpreter* instance;

    Status _status;

    PyDict* _builtins;
    FrameObject* _curFrame;
    PyObject* _retValue;  // �ó�Ա�����������л�ջ��������ʱ������callee�ķ���ֵ

public:
    
    // InterpreterҲ���õ���ģʽ������C++����������Ҫ��ʱ���ʽ�����ʵ������
    static Interpreter* getInstance() {
        if (instance == nullptr) {
            instance = new Interpreter();
        }
        return instance;
    }

    void run(CodeObject* code);

    void evalFrame();

    void compareTwoPythonObjects(PyObject* lhs, PyObject* rhs, uint16_t);

    void makeFunction(int16_t defaultArgCount, bool isClosureFunc);

    void entryIntoNewFrame(PyObject* callableObject, PyList* rawArgs, 
        uint8_t argNumber_pos, uint8_t argNumber_kw);

    void destroyCurFrame();
    void exitFromCurFrame();

    PyObject* callVirtual(PyObject* callable, PyList* args);

    FrameObject* getCurrentFrame();

    // GC��ؽӿ�
    void oops_do(OopClosure* closure);
};

#endif