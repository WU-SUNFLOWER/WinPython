#include "MethodKlass.hpp"
#include "PyMethod.hpp"
#include <cstdio>

MethodKlass* MethodKlass::instance = nullptr;

void MethodKlass::print(const PyObject* lhs) const {
    checkLegalPyObject(lhs, this);
    const PyMethod* _lhs = static_cast<const PyMethod*>(lhs);
    printf("<method: ");
    _lhs->getFunc()->getName()->print();
    printf(">");
}