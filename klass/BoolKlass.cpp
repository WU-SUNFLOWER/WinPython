#include "BoolKlass.hpp"
#include "PyBool.hpp"
#include <stdio.h>

BoolKlass* BoolKlass::instance = nullptr;

BoolKlass* BoolKlass::getInstance() {
    if (instance == nullptr) {
        instance = new BoolKlass();
    }
    return instance;
}

void BoolKlass::print(const PyObject* obj) const {
    const PyBool* obj1 = static_cast<const PyBool*>(obj);
    printf("%s", obj1->getValue() ? "True" : "False");
}
