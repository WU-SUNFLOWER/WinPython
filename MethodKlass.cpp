#include "MethodKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyMethod.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

MethodKlass* MethodKlass::instance = nullptr;

void MethodKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyMethod* _lhs = static_cast<const PyMethod*>(lhs);
    printf("<method: ");
    _lhs->getFunc()->getName()->print();
    printf(">");
}

void MethodKlass::initialize() {

    (new PyTypeObject())->setOwnKlass(this);

    setName(StringTable::str_method);

    setSuperKlass(ObjectKlass::getInstance());
}