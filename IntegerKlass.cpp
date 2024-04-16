#include "IntegerKlass.hpp"
#include "Universe.hpp"
#include "PyInteger.hpp"
#include <cstdio>
#include <cstdlib>

IntegerKlass* IntegerKlass::instance = nullptr;

void IntegerKlass::print(const PyObject* lhs, int flags) const {
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    Klass::checkLegalPyObject(_lhs, this);
    printf("%d", _lhs->getValue());
}

PyObject* IntegerKlass::less(const PyObject* lhs, const PyObject* rhs) const {
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始比大小
    return _lhs->getValue() < _rhs->getValue() ?
        Universe::PyTrue : Universe::PyFalse;
}

PyObject* IntegerKlass::less_equal(const PyObject* lhs, const PyObject* rhs) const {
    return packBoolean(isTrue(equal(lhs, rhs)) || isTrue(less(lhs, rhs)));
}

PyObject* IntegerKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    // 数据类型不同的两个python对象肯定不相等
    if (lhs->getKlass() != rhs->getKlass()) return Universe::PyFalse;
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始比大小
    return _lhs->getValue() == _rhs->getValue() ? 
        Universe::PyTrue : Universe::PyFalse;
}

PyObject* IntegerKlass::greater_equal(const PyObject* lhs, const PyObject* rhs) const {
    return packBoolean(isTrue(greater(lhs, rhs)) || isTrue(equal(lhs, rhs)));
}

PyObject* IntegerKlass::greater(const PyObject* lhs, const PyObject* rhs) const {
    return packBoolean(isFalse(less_equal(lhs, rhs)));
}

PyObject* IntegerKlass::not_equal(const PyObject* lhs, const PyObject* rhs) const {
    return packBoolean(isFalse(equal(lhs, rhs)));
}

PyObject* IntegerKlass::add(const PyObject* lhs, const PyObject* rhs) const {
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始计算
    return new PyInteger(_lhs->getValue() + _rhs->getValue());
}

PyObject* IntegerKlass::sub(const PyObject* lhs, const PyObject* rhs) const {
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始计算
    return new PyInteger(_lhs->getValue() - _rhs->getValue());
}

PyObject* IntegerKlass::mul(const PyObject* lhs, const PyObject* rhs) const
{
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始计算
    return new PyInteger(_lhs->getValue() * _rhs->getValue());
}

PyObject* IntegerKlass::div(const PyObject* lhs, const PyObject* rhs) const
{
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始计算
    return new PyInteger(_lhs->getValue() / _rhs->getValue());
}

PyObject* IntegerKlass::mod(const PyObject* lhs, const PyObject* rhs) const
{
    // 检查传入的两对象是否为PyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // 检查通过，开始计算
    return new PyInteger(_lhs->getValue() % _rhs->getValue());
}
