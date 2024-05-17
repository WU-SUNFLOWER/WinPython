#include "FloatKlass.hpp"
#include "PyFloat.hpp"  // 包含PyFloat的定义
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
        std::cerr << "输入的不是一个PyFloat类型" << std::endl;
        return;
    }
    const PyFloat* obj1 = static_cast<const PyFloat*>(obj);
    std::cout << obj1->getValue();

    if (flags & 1) {
        std::cout << std::endl;
    }
}
