#include "NativeFunctionKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

void NativeFunctionKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<native function: ");
    _lhs->funcName->print();
    printf(">");
}

void NativeFunctionKlass::initialize() {

    (new PyTypeObject())->setOwnKlass(this);

    setName(StringTable::str_builtin_func);

    setSuperKlass(ObjectKlass::getInstance());
}