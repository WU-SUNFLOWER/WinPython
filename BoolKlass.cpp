#include "BoolKlass.hpp"
#include "PyBool.hpp"
#include <stdio.h>
#include "PyTypeObject.hpp"
#include "StringTable.hpp"
#include "PyDict.hpp"
#include "ObjectKlass.hpp"

BoolKlass* BoolKlass::instance = nullptr;

BoolKlass* BoolKlass::getInstance() {
    if (instance == nullptr) {
        instance = new BoolKlass();
    }
    return instance;
}

void BoolKlass::print(const PyObject* obj,int) const {
    const PyBool* obj1 = static_cast<const PyBool*>(obj);
    printf("%s", obj1->getValue() ? "True" : "False");
}

void BoolKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_bool);
    addSuper(ObjectKlass::getInstance());
    orderSupers();
}