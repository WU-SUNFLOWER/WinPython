#include "PyTypeObject.hpp"
#include "TypeKlass.hpp"

PyTypeObject::PyTypeObject() {
    own_klass = nullptr;
    setKlass(TypeKlass::getInstance());
}

void PyTypeObject::setOwnKlass(Klass* klass) {
    // ʵ��C++ Klass��Python Type Object��˫���
    own_klass = klass;
    klass->setTypeObject(this);
}

Klass* PyTypeObject::getOwnKlass() const {
    return own_klass;
}
