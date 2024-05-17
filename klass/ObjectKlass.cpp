#include "ObjectKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyObject.hpp"
#include "StringTable.hpp"

ObjectKlass* ObjectKlass::instance = nullptr;

ObjectKlass::ObjectKlass() {
}

ObjectKlass* ObjectKlass::getInstance() {
    if (instance == nullptr) {
        instance = new ObjectKlass();
    }
    return instance;
}

void ObjectKlass::initialize() {
    PyTypeObject* type = new PyTypeObject();
    type->setOwnKlass(this);
    setName(StringTable::str_object);
    orderSupers();
}

size_t ObjectKlass::getSize() {
    return sizeof(PyObject);
}

void ObjectKlass::oops_do(OopClosure* closure, PyObject* object) {
    // Do nothing.
}