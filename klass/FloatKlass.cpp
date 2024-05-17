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
    double lhsValue, rhsValue;

    if (isPyInteger(lhs) || isPyInteger(rhs))
    {
        if (isPyInteger(lhs)) {
            lhsValue = static_cast<double>(toRawInteger(lhs));
            const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
            rhsValue = _rhs->getValue();
        }

        else if (isPyInteger(rhs)) {
            rhsValue = static_cast<double>(toRawInteger(rhs));
            const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
            lhsValue = _lhs->getValue();
        }

        return new PyFloat(lhsValue + rhsValue);
    }


    else
    {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        return new PyFloat(_lhs->getValue() + _rhs->getValue());
    }

    return new PyFloat(lhsValue + rhsValue);
}

PyObject* FloatKlass::sub(const PyObject* lhs, const PyObject* rhs) const {
    double lhsValue, rhsValue;
    START_COUNT_TEMP_OBJECTS;
    if (isPyInteger(lhs) || isPyInteger(rhs))
    {
        if (isPyInteger(lhs)) {
            lhsValue = static_cast<double>(toRawInteger(lhs));
            const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
            rhsValue = _rhs->getValue();

        }
        

        else if (isPyInteger(rhs)) {
            rhsValue = static_cast<double>(toRawInteger(rhs));
            const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
            lhsValue = _lhs->getValue();
        }

        return new PyFloat(lhsValue - rhsValue);
        END_COUNT_TEMP_OBJECTS;
    }


    else
    {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        return new PyFloat(_lhs->getValue() - _rhs->getValue());
    }

    return new PyFloat(lhsValue - rhsValue);
}

PyObject* FloatKlass::mul(const PyObject* lhs, const PyObject* rhs) const {
    double lhsValue, rhsValue;

    if (isPyInteger(lhs) || isPyInteger(rhs))
    {
        if (isPyInteger(lhs)) {
            lhsValue = static_cast<double>(toRawInteger(lhs));
            const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
            rhsValue = _rhs->getValue();
        }

        else if (isPyInteger(rhs)) {
            rhsValue = static_cast<double>(toRawInteger(rhs));
            const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
            lhsValue = _lhs->getValue();
        }

        return new PyFloat(lhsValue * rhsValue);
    }


    else
    {
        const PyFloat* _lhs = static_cast<const PyFloat*>(lhs);
        const PyFloat* _rhs = static_cast<const PyFloat*>(rhs);
        return new PyFloat(_lhs->getValue() * _rhs->getValue());
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

PyObject* FloatKlass::floor_div(const PyObject* lhs, const PyObject* rhs) const
{
    return nullptr;
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



