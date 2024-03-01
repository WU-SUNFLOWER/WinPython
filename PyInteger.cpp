#include "PyInteger.hpp"
#include <cstdint>
#include <iostream>
#include "Universe.hpp"

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
    if (value < static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    }
    else {
        return Universe::PyFalse;
    }
}

PyObject* PyInteger::less_equal(PyObject* other) const {
    if (value <= static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    }
    else {
        return Universe::PyFalse;
    }
}

PyObject* PyInteger::equal(PyObject* other) const {
    if (value == static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    }
    else {
        return Universe::PyFalse;
    }
}

PyObject* PyInteger::greater_equal(PyObject* other) const {
    if (value >= static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    }
    else {
        return Universe::PyFalse;
    }
}

PyObject* PyInteger::greater(PyObject* other) const {
    if (value > static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject* PyInteger::not_equal(PyObject* other) const {
    if (value != static_cast<PyInteger*>(other)->value) {
        return Universe::PyTrue;
    }
    else {
        return Universe::PyFalse;
    }
}