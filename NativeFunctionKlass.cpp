#include "NativeFunctionKlass.hpp"
#include "PyFunction.hpp"
#include <cstdio>

NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

void NativeFunctionKlass::print(const PyObject* lhs) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<native function: ");
    _lhs->funcName->print();
    printf(">");
}