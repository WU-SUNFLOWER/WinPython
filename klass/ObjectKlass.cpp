#include "ObjectKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyObject.hpp"

ObjectKlass* ObjectKlass::instance = nullptr;

ObjectKlass::ObjectKlass() {
    setSuperKlass(nullptr);
    (new PyTypeObject())->setOwnKlass(this);
}

ObjectKlass* ObjectKlass::getInstance() {
    if (instance == nullptr) {
        instance = new ObjectKlass();
    }
    return instance;
}

size_t ObjectKlass::getSize() {
    return sizeof(PyObject);
}

void ObjectKlass::oops_do(OopClosure* closure, PyObject* object) {
    // Do nothing.
}