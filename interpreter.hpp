#ifndef Interpreter_Hpp
#define Interpreter_Hpp

#include "CodeObject.hpp"

class Interpreter {
private:
    ArrayList<PyObject*>* runtimeStack;
    ArrayList<PyObject*>* consts;
public:
    ~Interpreter();
    void run(CodeObject* code);
};

#endif