#include "ObjectKlass.hpp"
#include "PyTypeObject.hpp"

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
