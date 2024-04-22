#include "FunctionKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

FunctionKlass* FunctionKlass::instance = nullptr;

void FunctionKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_function);
    setSuperKlass(ObjectKlass::getInstance());
}

void FunctionKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<function: ");
    _lhs->funcName->print();
    printf(">");
}