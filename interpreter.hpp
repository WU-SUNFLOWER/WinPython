#ifndef Interpreter_Hpp
#define Interpreter_Hpp

#include "CodeObject.hpp"
#include "PyInteger.hpp"
#include "Universe.hpp"
#include "bytecode.hpp"
#include <iostream>

class Interpreter {
private:
    ArrayList<PyObject*>* runtimeStack;
    ArrayList<PyObject*>* consts;
public:
    ~Interpreter();
    void run(CodeObject* code);
};

#endif