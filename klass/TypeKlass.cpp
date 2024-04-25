#include "TypeKlass.hpp"
#include "PyObject.hpp"
#include "PyTypeObject.hpp"
#include "PyString.hpp"
#include <cassert>
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"
#include "Universe.hpp"
#include "PyDict.hpp"
#include "StringKlass.hpp"
#include <stdlib.h>

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
    printf("<class \"");

    const Klass* ownKlass = typeObject->getOwnKlass();
    const PyDict* attrDict = ownKlass->getKlassDict();
    if (attrDict != nullptr) {
        const PyObject* moduleName = attrDict->get(StringTable::str_mod);
        if (moduleName != nullptr) {
            moduleName->print(FLAG_PyString_PRINT_RAW);
            putchar('.');
        }
    }

    ownKlass->getName()->print(FLAG_PyString_PRINT_RAW);
    printf("\">");
}

void TypeKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_type);
    setSuperKlass(ObjectKlass::getInstance());
}

PyObject* TypeKlass::getattr(PyObject* object, PyObject* attr) {
    checkLegalPyObject(object, this);
    assert(attr->getKlass() == StringKlass::getInstance());
    
    PyTypeObject* cls = static_cast<PyTypeObject*>(object);
    Klass* ownKlass = cls->getOwnKlass();
    PyDict* klassDict = ownKlass->getKlassDict();
    assert(klassDict != nullptr);

    PyObject* ret = klassDict->get(attr);
    if (ret != nullptr) {
        return ret;
    }
    else {
        printf("type object '%s' has no attribute '%s'",
            ownKlass->getName()->getValue(),
            static_cast<PyString*>(attr)->getValue());
        exit(-1);
    }

}

void TypeKlass::setattr(PyObject* object, PyObject* attr, PyObject* value) {
    checkLegalPyObject(object, this);
    assert(attr->getKlass() == StringKlass::getInstance());

    PyTypeObject* cls = static_cast<PyTypeObject*>(object);
    PyDict* klassDict = cls->getOwnKlass()->getKlassDict();
    assert(klassDict != nullptr);
    klassDict->set(attr, value);
}

PyObject* TypeKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    return lhs == rhs ? Universe::PyTrue : Universe::PyFalse;
}
