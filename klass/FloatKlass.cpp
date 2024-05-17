#include "FloatKlass.hpp"
#include "PyFloat.hpp"  // ����PyFloat�Ķ���
#include <iostream>

FloatKlass* FloatKlass::instance = nullptr;

FloatKlass* FloatKlass::getInstance() {
    if (instance == nullptr) {
        instance = new FloatKlass();
    }
    return instance;
}

void FloatKlass::print(const PyObject* obj, int flags) const {
    if (obj->getKlass() != FloatKlass::getInstance()) {
        std::cerr << "����Ĳ���һ��PyFloat����" << std::endl;
        return;
    }
    const PyFloat* obj1 = static_cast<const PyFloat*>(obj);
    std::cout << obj1->getValue();

    if (flags & 1) {
        std::cout << std::endl;
    }
}
