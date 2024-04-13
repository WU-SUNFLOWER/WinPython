#include "PyObject.hpp"
#include "Universe.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "PyFunction.hpp"
#include "PyMethod.hpp"
#include <cstdlib>
#include <cstdio>

void PyObject::print() const
{
    return getKlass()->print(this);
}

PyObject* PyObject::add(PyObject* other) const {
    return getKlass()->add(this, other);
}

PyObject* PyObject::sub(PyObject* other) const {
    return getKlass()->sub(this, other);
}

PyObject* PyObject::mul(PyObject* other) const {
    return getKlass()->mul(this, other);
}

PyObject* PyObject::div(PyObject* other) const {
    return getKlass()->div(this, other);
}

PyObject* PyObject::mod(PyObject* other) const {
    return getKlass()->mod(this, other);
}

PyObject* PyObject::less(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->less(this, other);
}

PyObject* PyObject::less_equal(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->less_equal(this, other);
}

PyObject* PyObject::equal(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->equal(this, other);
}

PyObject* PyObject::greater_equal(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->greater_equal(this, other);
}

PyObject* PyObject::greater(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->greater(this, other);
}

PyObject* PyObject::not_equal(const PyObject* other) const {
    if (getKlass() != other->getKlass()) {
        puts("You can't compare objects of different data types.");
        exit(-1);
    }
    return getKlass()->not_equal(this, other);
}

PyObject* PyObject::len() const {
    return getKlass()->len(this);
}

PyObject* PyObject::getattr(PyObject* attr) const {
    PyObject* ret = getKlass()->getKlassDict()->get(attr);
    if (ret == Universe::PyNone) return ret;
    
    // 如果尝试获取某个对象的方法函数
    // 则将其由原生函数打包成PyMethod
    const Klass* retKlass = ret->getKlass();
    if (retKlass == FunctionKlass::getInstance()
        || retKlass == NativeFunctionKlass::getInstance()) {
        ret = new PyMethod(static_cast<PyFunction*>(ret), 
            const_cast<PyObject*>(this));
    }
    
    return ret;
}

PyObject* PyObject::subscr(PyObject* subscription) {
    return getKlass()->subscr(this, subscription);
}

PyObject* PyObject::store_subscr(PyObject* subscription, PyObject* newObject) {
    return getKlass()->store_subscr(this, subscription, newObject);
}

PyObject* PyObject::delete_subscr(PyObject* subscription) {
    return getKlass()->delete_subscr(this, subscription);
}

PyObject* PyObject::has(PyObject* target) {
    return getKlass()->has(this, target);
}
