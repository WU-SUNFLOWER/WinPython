#include "StringKlass.hpp"
#include "Universe.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"
#include "PyDict.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "nativeFunctions.hpp"

StringKlass* StringKlass::instance = nullptr;

void StringKlass::initialize() {
    PyDict* dict = new PyDict();
    dict->set(new PyString("upper"),
        PackNativeFunc(NativeFunction::string_upper));
    setKlassDict(dict);
}

void StringKlass::print(const PyObject* lhs) const {
    checkLegalPyObject(lhs, this);
    const PyString* _lhs = static_cast<const PyString*>(lhs);
    size_t length = _lhs->getLength();
    const char* ptr = reinterpret_cast<const char*>(_lhs->getValue());

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

PyObject* StringKlass::len(const PyObject* strObject) const {
    const PyString* _lhs = static_cast<const PyString*>(strObject);
    int32_t charLength = 0;
    size_t bufLength = _lhs->getLength();
    const uint8_t* ptr = _lhs->getValue();
    for (size_t i = 0; i < bufLength;) {
        if ((ptr[i] & 0xF0) == 0xE0) {
            // 1110xxxx 表示是三字节的UTF-8字符
            i += 3;
        }
        else if ((ptr[i] & 0xE0) == 0xC0) {
            // 110xxxxx 表示是两字节的UTF-8字符
            i += 2;
        }
        else if ((ptr[i] & 0x80) == 0x00) {
            // 0xxxxxxx 表示是一个字节的UTF-8字符
            i += 1;
        }
        else {
            // 对于四字节的UTF-8字符处理
            i += 4;
        }
        ++charLength;
    }
    return new PyInteger(charLength);
}

PyObject* StringKlass::subscr(PyObject* object, PyObject* subscription) const {
    assert(object->getKlass() == this);
    assert(subscription->getKlass() == IntegerKlass::getInstance());
    PyString* strObject = reinterpret_cast<PyString*>(object);
    PyInteger* subscr = reinterpret_cast<PyInteger*>(subscription);
    const uint8_t* address = strObject->getValue() + subscr->getValue();
    PyString* result = new PyString(address, 1);
    return result;
}

PyObject* StringKlass::has(PyObject* object, PyObject* target) const {
    assert(object->getKlass() == this);
    assert(target->getKlass() == this);
    PyString* str = reinterpret_cast<PyString*>(object);
    PyString* targetStr = reinterpret_cast<PyString*>(target);
    const char* result = strstr(
        reinterpret_cast<const char*>(str->getValue()),
        reinterpret_cast<const char*>(targetStr->getValue())
    );
    return result == nullptr ? Universe::PyFalse : Universe::PyTrue;
}