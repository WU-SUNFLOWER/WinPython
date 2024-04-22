#include "TypeKlass.hpp"
#include "PyObject.hpp"
#include "PyTypeObject.hpp"
#include "PyString.hpp"
#include <cassert>
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"
#include "Universe.hpp"

TypeKlass* TypeKlass::instance = nullptr;

TypeKlass* TypeKlass::getInstance() {
    if (instance == nullptr) {
        instance = new TypeKlass();
    }
    return instance;
}

void TypeKlass::print(const PyObject* object, int flag) const {
    checkLegalPyObject(object, this);
    const PyTypeObject* typeObject = reinterpret_cast<const PyTypeObject*>(object);
    printf("<class ");
    typeObject->getOwnKlass()->getName()->print();
    printf(">");
}

void TypeKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_type);
    setSuperKlass(ObjectKlass::getInstance());
}

PyObject* TypeKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    return lhs == rhs ? Universe::PyTrue : Universe::PyFalse;
}
