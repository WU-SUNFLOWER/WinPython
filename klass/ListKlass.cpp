#include "ListKlass.hpp"
#include <cstdio>
#include "StringKlass.hpp"
#include "IntegerKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyDict.hpp"
#include "Universe.hpp"
#include <algorithm>
#include "nativeFunctions.hpp"
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

ListKlass* ListKlass::instance = nullptr;

ListIteratorKlass* ListIteratorKlass::instance = nullptr;

ListKlass::ListKlass() {

}

size_t ListKlass::getSize() {
    return sizeof(PyList);
}

void ListKlass::initialize() {
    PyDict* dict = PyDict::createDict();

    dict->set(PyString::createString("append"),
        PackNativeFunc(NativeFunction::list_append));
    dict->set(PyString::createString("insert"),
        PackNativeFunc(NativeFunction::list_insert));
    dict->set(PyString::createString("index"),
        PackNativeFunc(NativeFunction::list_index));
    dict->set(PyString::createString("pop"),
        PackNativeFunc(NativeFunction::list_pop));
    dict->set(PyString::createString("remove"),
        PackNativeFunc(NativeFunction::list_remove));
    dict->set(PyString::createString("reverse"),
        PackNativeFunc(NativeFunction::list_reverse));

    setKlassDict(dict);

    // 实现C++ Klass和Python TypeObject的双向绑定
    (new PyTypeObject())->setOwnKlass(this);

    // 设置类名
    setName(StringTable::str_list);

    // 设置基类
    setSuperKlass(ObjectKlass::getInstance());
}

void ListKlass::print(const PyObject* lhs, int flags) const {
    const PyList* list = reinterpret_cast<const PyList*>(lhs);
    size_t length = list->getLength();
    putchar('[');
    for (size_t i = 0; i < length; ++i) {
        const PyObject* elem = list->get(i);
        if (isPyInteger(elem)) {
            printf("%lld", toRawInteger(elem));
        }
        else {
            elem->print();
        }
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
    PyList* result = PyList::createList(len_a + len_b);
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
    if (isPyInteger(rhs)) {
        const PyList* list = static_cast<const PyList*>(lhs);
        int64_t count = toRawInteger(rhs);
        size_t len = list->getLength();
        PyList* result = PyList::createList(count * len);
        while (count-- > 0) {
            for (size_t i = 0; i < len; ++i) {
                result->append(list->get(i));
            }
        }
        return result;
    }
    else {
        puts("can't multiply sequence by non-int of type 'list'");
        exit(-1);
    }
}

PyObject* ListKlass::len(const PyObject* o) const {
    const PyList* list = static_cast<const PyList*>(o);
    return toPyInteger(list->getLength());
}

PyObject* ListKlass::subscr(PyObject* object, PyObject* subscrption) const {

    checkLegalPyObject(object, this);

    PyList* list = reinterpret_cast<PyList*>(object);
    size_t pos;
    if (isPyInteger(subscrption)) {
        pos = toRawInteger(subscrption);
    }
    else {
        assert(subscrption->getKlass() == IntegerKlass::getInstance());
        PyInteger* subscr = reinterpret_cast<PyInteger*>(subscrption);
        pos = subscr->getValue();
    }

    // Python中不允许对越界下标进行访问
    if (pos >= list->getLength()) {
        puts("list index out of range");
        exit(-1);
    }
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

PyObject* ListKlass::getIter(PyObject* object) const {
    checkLegalPyObject(object, this);
    return new ListIterator(static_cast<PyList*>(object));
}

void ListKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    PyList* list = static_cast<PyList*>(object);
    closure->do_array_list(&list->_container);
}

void ListKlass::store_subscr(PyObject* object, 
    PyObject* subscription, PyObject* newObject
) const {
    checkLegalPyObject(object, this);

    PyList* list = reinterpret_cast<PyList*>(object);
    size_t pos;
    
    if (isPyInteger(subscription)) {
        pos = toRawInteger(subscription);
    }
    else {
        assert(subscription->getKlass() == IntegerKlass::getInstance());
        PyInteger* subscr = reinterpret_cast<PyInteger*>(subscription);
        pos = subscr->getValue();
    }

    // Python中不允许对越界下标进行访问
    if (pos >= list->getLength()) {
        puts("list index out of range");
        exit(-1);
    }

    // 重新赋值
    list->set(pos, newObject);
}

void ListKlass::delete_subscr(PyObject* object, 
    PyObject* subscription
) const {
    assert(object->getKlass() == this &&
        subscription->getKlass() == IntegerKlass::getInstance());
    PyList* list = reinterpret_cast<PyList*>(object);
    size_t pos;
    if (isPyInteger(subscription)) {
        pos = toRawInteger(subscription);
    }
    else {
        pos = reinterpret_cast<PyInteger*>(subscription)->getValue();
    }
    // Python中不允许对越界下标进行访问
    if (pos >= list->getLength()) {
        puts("list index out of range");
        exit(-1);
    }
    list->deleteByIndex(pos);
}

ListIteratorKlass::ListIteratorKlass() {
    PyDict* dict = PyDict::createDict();

    dict->set(StringTable::str_next,
        PackNativeFunc(NativeFunction::list_iterator_next));

    setKlassDict(dict);
}

PyObject* ListIteratorKlass::next(PyObject* object) const {
    checkLegalPyObject(object, this);
    ListIterator* iter = static_cast<ListIterator*>(object);
    PyList* list = iter->getOwner();
    int count = iter->getCount();
    if (count < list->getLength()) {
        PyObject* nextElem = list->get(count);
        iter->increaseCount();
        return nextElem;
    }
    else {
        return nullptr;
    }
}

void ListIteratorKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    ListIterator* iter = static_cast<ListIterator*>(object);
    closure->do_oop(reinterpret_cast<PyObject**>(&iter->_owner));
}
