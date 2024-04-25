#include "PyTypeObject.hpp"
#include "TypeKlass.hpp"

PyTypeObject::PyTypeObject() {
    own_klass = nullptr;
    setKlass(TypeKlass::getInstance());
}

void PyTypeObject::setOwnKlass(Klass* klass) {
    // 实现C++ Klass和Python Type Object的双向绑定
    own_klass = klass;
    klass->setTypeObject(this);
}

Klass* PyTypeObject::getOwnKlass() const {
    return own_klass;
}
