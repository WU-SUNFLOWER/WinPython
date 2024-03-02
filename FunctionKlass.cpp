#include "FunctionKlass.hpp"
#include "PyFunction.hpp"
#include <cstdio>

FunctionKlass* FunctionKlass::instance = nullptr;

void FunctionKlass::print(const PyObject* lhs) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<function: ");
    _lhs->funcName->print();
    printf(">");
}