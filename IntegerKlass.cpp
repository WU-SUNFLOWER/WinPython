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
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ�ȴ�С
    return _lhs->getValue() < _rhs->getValue() ?
        Universe::PyTrue : Universe::PyFalse;
}

PyObject* IntegerKlass::less_equal(const PyObject* lhs, const PyObject* rhs) const {
    return packBoolean(isTrue(equal(lhs, rhs)) || isTrue(less(lhs, rhs)));
}

PyObject* IntegerKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    // �������Ͳ�ͬ������python����϶������
    if (lhs->getKlass() != rhs->getKlass()) return Universe::PyFalse;
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ�ȴ�С
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
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ����
    return new PyInteger(_lhs->getValue() + _rhs->getValue());
}

PyObject* IntegerKlass::sub(const PyObject* lhs, const PyObject* rhs) const {
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ����
    return new PyInteger(_lhs->getValue() - _rhs->getValue());
}

PyObject* IntegerKlass::mul(const PyObject* lhs, const PyObject* rhs) const
{
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ����
    return new PyInteger(_lhs->getValue() * _rhs->getValue());
}

PyObject* IntegerKlass::div(const PyObject* lhs, const PyObject* rhs) const
{
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ����
    return new PyInteger(_lhs->getValue() / _rhs->getValue());
}

PyObject* IntegerKlass::mod(const PyObject* lhs, const PyObject* rhs) const
{
    // ��鴫����������Ƿ�ΪPyInteger
    const PyInteger* _lhs = static_cast<const PyInteger*>(lhs);
    const PyInteger* _rhs = static_cast<const PyInteger*>(rhs);
    checkLegalPyObject_DB(_lhs, _rhs);
    // ���ͨ������ʼ����
    return new PyInteger(_lhs->getValue() % _rhs->getValue());
}
