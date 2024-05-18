#include "FloatKlass.hpp"
#include "PyFloat.hpp"  // 包含PyFloat的定义
#include "PyTypeObject.hpp"
#include "ObjectKlass.hpp"
#include "StringTable.hpp"
#include <iostream>

#include "Universe.hpp"

FloatKlass* FloatKlass::instance = nullptr;

FloatKlass* FloatKlass::getInstance() {
    if (instance == nullptr) {
        instance = new FloatKlass();
    }
    return instance;
}

void FloatKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_float);
    addSuper(ObjectKlass::getInstance());
    orderSupers();
}

PyObject* FloatKlass::add(const PyObject* lhs, const PyObject* rhs) const {
    double lhsValue = 0;
    double rhsValue = 0;

    if (isPyInteger(lhs)) {
        lhsValue = static_cast<double>(toRawInteger(lhs));
    }
    else if (lhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        lhsValue = _lhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    if (isPyInteger(rhs)) {
        rhsValue = static_cast<double>(toRawInteger(rhs));
    }
    else if (rhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        rhsValue = _rhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    return new PyFloat(lhsValue + rhsValue);
}

PyObject* FloatKlass::sub(const PyObject* lhs, const PyObject* rhs) const {
    double lhsValue = 0;
    double rhsValue = 0;

    if (isPyInteger(lhs)) {
        lhsValue = static_cast<double>(toRawInteger(lhs));
    }
    else if (lhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        lhsValue = _lhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    if (isPyInteger(rhs)) {
        rhsValue = static_cast<double>(toRawInteger(rhs));
    }
    else if (rhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        rhsValue = _rhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    return new PyFloat(lhsValue - rhsValue);
}

PyObject* FloatKlass::mul(const PyObject* lhs, const PyObject* rhs) const {
    double lhsValue = 0;
    double rhsValue = 0;

    if (isPyInteger(lhs)) {
        lhsValue = static_cast<double>(toRawInteger(lhs));
    }
    else if (lhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        lhsValue = _lhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    if (isPyInteger(rhs)) {
        rhsValue = static_cast<double>(toRawInteger(rhs));
    }
    else if (rhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        rhsValue = _rhs->getValue();
    }
    else {
        printf("fail to convert to float.\n");
        exit(-1);
    }

    return new PyFloat(lhsValue * rhsValue);
}

PyObject* FloatKlass::div(const PyObject* lhs, const PyObject* rhs) const {

    if (lhs->getKlass() == FloatKlass::getInstance() && isPyInteger(rhs))
    {
        double rhsValue;
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        rhsValue = static_cast<double>toRawInteger(rhs);
        if (rhsValue != 0.0) {
            return new PyFloat(_lhs->getValue() / rhsValue);
        }
        else {
            printf(" division by zero");
            exit(-1);
        }

    }

    else if (lhs->getKlass() == FloatKlass::getInstance() && rhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        if (_rhs->getValue() != 0.0) {
            return new PyFloat(_lhs->getValue() / _rhs->getValue());
        }
        else {
            printf(" division by zero");
            exit(-1);
        }
    }
    else if (lhs->getKlass() == FloatKlass::getInstance() && isPyInteger(rhs))
    {
        double rhsValue;
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        rhsValue = static_cast<double>toRawInteger(rhs);
        if (rhsValue != 0.0) {
            return new PyFloat(_lhs->getValue() / rhsValue);
        }
        else {
            printf(" division by zero");
            exit(-1);
        }
        
    }
    else
    {
        printf("division with illegal types");
        exit(-1);
    }
   
}

PyObject* FloatKlass::floor_div(const PyObject* lhs, const PyObject* rhs) const {
    checkLegalPyObject(lhs, this);
    const PyFloat* lvalue = static_cast<const PyFloat*>(lhs);
    if (isPyInteger(rhs)) {
        if (toRawInteger(rhs) == 0) {
            printf("division by zero");
            exit(-1);
        }
        return new PyFloat(floor(lvalue->getValue() / toRawInteger(rhs)));
    }
    else if (rhs->getKlass() == FloatKlass::getInstance()) {
        const PyFloat* rvalue = static_cast<const PyFloat*>(rhs);
        if (rvalue->getValue() == 0.0) {
            printf("division by zero");
            exit(-1);
        }
        return new PyFloat(floor(lvalue->getValue() / rvalue->getValue()));
    }
    else {
        printf("can't divide");
        exit(-1);
    }
}


void FloatKlass::print(const PyObject* obj, int flags) const {
    checkLegalPyObject(obj, this);
    const PyFloat* obj1 = static_cast<const PyFloat*>(obj);
    printf("%lf", obj1->getValue());
}

PyObject* FloatKlass::isBoolTrue(PyObject* object) {
    PyFloat* float_obj = static_cast<PyFloat*>(object);
    if (float_obj->getValue() == 0.0) {
        return Universe::PyFalse;
    }

    return Universe::PyTrue;
}



