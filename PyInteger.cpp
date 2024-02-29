#include "PyInteger.hpp"

void PyInteger::print() const {
    printf("%d", value);
}

int32_t PyInteger::getValue() const {
    return value;
}

PyObject* PyInteger::add(PyObject* other) const {
    return new PyInteger(value + static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::less(PyObject* other) const {
    return new PyInteger(value < static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::less_equal(PyObject* other) const {
    return new PyInteger(value <= static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::equal(PyObject* other) const {
    return new PyInteger(value == static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::greater_equal(PyObject* other) const {
    return new PyInteger(value >= static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::greater(PyObject* other) const {
    return new PyInteger(value > static_cast<PyInteger*>(other)->value);
}

PyObject* PyInteger::not_equal(PyObject* other) const {
    return new PyInteger(value != static_cast<PyInteger*>(other)->value);
}