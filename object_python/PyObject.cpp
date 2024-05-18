#include "PyObject.hpp"
#include "Universe.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "PyFunction.hpp"
#include "PyMethod.hpp"
#include "PyDict.hpp"
#include "PyTypeObject.hpp"
#include <cstdlib>
#include <cstdio>
#include "TypeKlass.hpp"

PyString* PyObject::getKlassName() const {
    assert(klass != nullptr);
    return getKlass()->getName();
}

const uint8_t* PyObject::getKlassNameAsString() const {
    assert(klass != nullptr);
    return getKlassName()->getValue();
}

PyDict* PyObject::getSelfDict() {
    return _self_dict;
}

PyDict* PyObject::initSelfDict() {
    return (_self_dict = PyDict::createDict());
}

void* PyObject::operator new(size_t size, bool isInMeta) {
    return isInMeta ? 
        Universe::PyHeap->allocateMeta(size) :
        Universe::PyHeap->allocate(size);
}

void PyObject::operator delete(void*, bool) {
    puts("You can't delete a python object directly.");
    exit(-1);
}

void PyObject::print(int flags) const {
    return getKlass()->print(this, flags);
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

PyObject* PyObject::floor_div(PyObject* other) const
{
    return getKlass()->floor_div(this, other);
}

PyObject* PyObject::inplace_add(PyObject* other) {
    return getKlass()->inplace_add(this, other);
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
    if (isPyInteger(this)) {
        return isPyInteger(other) ? packBoolean(this == other) : Universe::PyFalse;
    }
    else if (isPyInteger(other)) {
        return Universe::PyFalse;
    }
    else {
        if (getKlass() != other->getKlass()) {
            return Universe::PyFalse;
        }
        return getKlass()->equal(this, other);
    }
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

PyObject* PyObject::getattr(PyObject* attr) {
    return getKlass()->getattr(this, attr);
}

void PyObject::setattr(PyObject* attr, PyObject* value) {
    getKlass()->setattr(this, attr, value);
}

PyObject* PyObject::subscr(PyObject* subscription) {
    return getKlass()->subscr(this, subscription);
}

void PyObject::store_subscr(PyObject* subscription, PyObject* newObject) {
    getKlass()->store_subscr(this, subscription, newObject);
}

void PyObject::delete_subscr(PyObject* subscription) {
    getKlass()->delete_subscr(this, subscription);
}

PyObject* PyObject::has(PyObject* target) {
    return getKlass()->has(this, target);
}

PyObject* PyObject::getIter() {
    return getKlass()->getIter(this);
}

PyObject* PyObject::isBoolTrue() {
    return getKlass()->isBoolTrue(this);
}

PyObject* PyObject::next() {
    return getKlass()->next(this);
}

PyObject* PyObject::isinstance(PyTypeObject* type) const {
    return this->getKlass()->mro()->has(type);
}

/* GC相关接口 开始 */
void* PyObject::getNewAddr() {
    if ((_mark_word & 0x2) == 0x2) {
        return reinterpret_cast<void*>(_mark_word & ~7);
    }
    return nullptr;
}

void PyObject::setNewAddr(void* addr) {
    if (!addr) return;
    _mark_word = reinterpret_cast<uintptr_t>(addr) | 0x2;
}

size_t PyObject::getSize() {
    return getKlass()->getSize();
}

void PyObject::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&_self_dict));
    getKlass()->oops_do(closure, this);
}
/* GC相关接口 结束 */