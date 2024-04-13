#include "ListKlass.hpp"
#include <cstdio>
#include "StringKlass.hpp"
#include "IntegerKlass.hpp"
#include "PyInteger.hpp"
#include "Universe.hpp"
#include <algorithm>

ListKlass* ListKlass::instance = nullptr;

void ListKlass::print(const PyObject* lhs) const {
    const PyList* list = reinterpret_cast<const PyList*>(lhs);
    size_t length = list->getLength();
    putchar('[');
    for (size_t i = 0; i < length; ++i) {
        const PyObject* elem = list->get(i);
        bool isPyString = elem->getKlass() == StringKlass::getInstance();
        if (isPyString) putchar('"');
        list->get(i)->print();
        if (isPyString) putchar('"');
        if (i < length - 1) printf(", ");
    }
    putchar(']');
}

PyObject* ListKlass::less(const PyObject* lhs, const PyObject* rhs) const {
    const PyList* _lhs = static_cast<const PyList*>(lhs);
    const PyList* _rhs = static_cast<const PyList*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);

    size_t len_lhs = _lhs->getLength();
    size_t len_rhs = _rhs->getLength();
    size_t length = std::min(len_lhs, len_rhs);

    for (size_t i = 0; i < length; ++i) {
        const PyObject* elem_lhs = _lhs->get(i);
        const PyObject* elem_rhs = _rhs->get(i);
        if (elem_lhs->less(elem_rhs) == Universe::PyTrue) {
            return Universe::PyTrue;
        }
        else if (elem_lhs->greater(elem_rhs) == Universe::PyTrue) {
            return Universe::PyFalse;
        }
    }

    return len_lhs < len_rhs ? Universe::PyTrue : Universe::PyFalse;
}

PyObject* ListKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    const PyList* _lhs = static_cast<const PyList*>(lhs);
    const PyList* _rhs = static_cast<const PyList*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);

    size_t len_lhs = _lhs->getLength();
    size_t len_rhs = _rhs->getLength();

    if (len_lhs != len_rhs) return Universe::PyFalse;

    for (size_t i = 0; i < len_lhs; ++i) {
        const PyObject* elem_lhs = _lhs->get(i);
        const PyObject* elem_rhs = _rhs->get(i);
        if (elem_lhs->equal(elem_rhs) != Universe::PyTrue) {
            return Universe::PyFalse;
        }
    }

    return Universe::PyTrue;
}

PyObject* ListKlass::less_equal(const PyObject* lhs,
    const PyObject* rhs
) const {
    return packBoolean(isTrue(less(lhs, rhs)) || isTrue(equal(lhs, rhs)));
}

PyObject* ListKlass::greater_equal(const PyObject* lhs, const PyObject* rhs) const
{
    return packBoolean(isTrue(greater(lhs, rhs)) || isTrue(equal(lhs, rhs)));
}

PyObject* ListKlass::greater(const PyObject* lhs, const PyObject* rhs) const
{
    return packBoolean(isFalse(less_equal(lhs, rhs)));
}

PyObject* ListKlass::not_equal(const PyObject* lhs, const PyObject* rhs) const
{
    return takeNot(equal(lhs, rhs));
}

PyObject* ListKlass::add(const PyObject* lhs, const PyObject* rhs) const {
    checkLegalPyObject_DB(lhs, rhs);
    const PyList* a = static_cast<const PyList*>(lhs);
    const PyList* b = static_cast<const PyList*>(rhs);
    size_t len_a = a->getLength();
    size_t len_b = b->getLength();
    PyList* result = new PyList(len_a + len_b);
    for (size_t i = 0; i < len_a; ++i) {
        result->append(a->get(i));
    }
    for (size_t i = 0; i < len_b; ++i) {
        result->append(b->get(i));
    }
    return result;
}

PyObject* ListKlass::mul(const PyObject* lhs, const PyObject* rhs) const {
    checkLegalPyObject(lhs, ListKlass::getInstance());
    checkLegalPyObject(rhs, IntegerKlass::getInstance());
    const PyList* list = static_cast<const PyList*>(lhs);
    const PyInteger* integer = static_cast<const PyInteger*>(rhs);
    int count = integer->getValue();
    size_t len = list->getLength();
    PyList* result = new PyList(count * len);
    while (count-- > 0) {
        for (size_t i = 0; i < len; ++i) {
            result->append(list->get(i));
        }
    }
    return result;
}

PyObject* ListKlass::subscr(PyObject* object, PyObject* subscrption) const {
    assert(object->getKlass() == this);
    assert(subscrption->getKlass() == IntegerKlass::getInstance());
    PyList* list = reinterpret_cast<PyList*>(object);
    PyInteger* subscr = reinterpret_cast<PyInteger*>(subscrption);
    size_t pos = subscr->getValue();
    // Python中不允许对越界下标进行访问
    assert(pos < list->getLength());
    return list->get(pos);
}

PyObject* ListKlass::has(PyObject* object, PyObject* target) const {
    assert(object->getKlass() == this);
    PyList* list = reinterpret_cast<PyList*>(object);
    size_t length = list->getLength();
    for (size_t i = 0; i < length; ++i) {
        if (list->get(i)->equal(target) == Universe::PyTrue) {
            return Universe::PyTrue;
        }
    }
    return Universe::PyFalse;
}

PyObject* ListKlass::store_subscr(PyObject* object, 
    PyObject* subscription, PyObject* newObject
) const {
    assert(object->getKlass() == this &&
        subscription->getKlass() == IntegerKlass::getInstance());
    PyList* list = reinterpret_cast<PyList*>(object);
    PyInteger* subscr = reinterpret_cast<PyInteger*>(subscription);
    size_t pos = subscr->getValue();
    // Python中不允许对越界下标进行访问
    assert(pos < list->getLength());
    // 重新赋值
    list->set(pos, newObject);
    return Universe::PyNone;
}

PyObject* ListKlass::delete_subscr(PyObject* object, 
    PyObject* subscription
) const {
    assert(object->getKlass() == this &&
        subscription->getKlass() == IntegerKlass::getInstance());
    PyList* list = reinterpret_cast<PyList*>(object);
    PyInteger* subscr = reinterpret_cast<PyInteger*>(subscription);
    size_t pos = subscr->getValue();
    // Python中不允许对越界下标进行访问
    assert(pos < list->getLength());
    list->deleteByIndex(pos);
    return Universe::PyNone;
}
