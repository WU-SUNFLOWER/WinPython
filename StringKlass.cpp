#include "StringKlass.hpp"
#include "Universe.hpp"
#include "PyString.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

StringKlass* StringKlass::instance = nullptr;

void StringKlass::print(const PyObject* lhs) const {
    checkLegalPyObject(lhs, this);
    const PyString* _lhs = static_cast<const PyString*>(lhs);
    size_t length = _lhs->getLength();
    const uint8_t* ptr = _lhs->getValue();
    for (size_t i = 0; i < length; ++i) {
        putchar(ptr[i]);
    }
}

PyObject* StringKlass::less(const PyObject* lhs, const PyObject* rhs) const
{
    const PyString* _lhs = static_cast<const PyString*>(lhs);
    const PyString* _rhs = static_cast<const PyString*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);

    size_t llen = _lhs->getLength(), rlen = _rhs->getLength();
    size_t minLen = llen > rlen ? rlen : llen;
    const char* lptr = reinterpret_cast<const char*>(_lhs->getValue());
    const char* rptr = reinterpret_cast<const char*>(_rhs->getValue());

    int cmp = strncmp(lptr, rptr, minLen);
    // lhs < rhs
    if (cmp < 0) {
        return Universe::PyTrue;
    }
    // lhs > rhs
    else if (cmp > 0) {
        return Universe::PyFalse;
    }
    else {
        return packBoolean(llen < rlen);
    }
}

PyObject* StringKlass::less_equal(const PyObject* lhs, const PyObject* rhs) const
{
    return packBoolean(isTrue(less(lhs, rhs)) || isTrue(equal(lhs, rhs)));
}

PyObject* StringKlass::equal(const PyObject* lhs, const PyObject* rhs) const
{
    if (lhs->getKlass() != rhs->getKlass()) return Universe::PyFalse;
    
    const PyString* _lhs = static_cast<const PyString*>(lhs);
    const PyString* _rhs = static_cast<const PyString*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);

    size_t llen = _lhs->getLength(), rlen = _rhs->getLength();
    
    if (llen != rlen) return Universe::PyFalse;
    
    return packBoolean(strncmp(
        reinterpret_cast<const char*>(_lhs->getValue()),
        reinterpret_cast<const char*>(_rhs->getValue()),
        llen
        ) == 0);
}

PyObject* StringKlass::greater_equal(const PyObject* lhs, const PyObject* rhs) const
{
    return packBoolean(isTrue(greater(lhs, rhs)) || isTrue(equal(lhs, rhs)));
}

PyObject* StringKlass::greater(const PyObject* lhs, const PyObject* rhs) const
{
    return packBoolean(isFalse(less_equal(lhs, rhs)));
}

PyObject* StringKlass::not_equal(const PyObject* lhs, const PyObject* rhs) const
{
    return takeNot(equal(lhs, rhs));
}
