#ifndef PYFLOAT_HPP
#define PYFLOAT_HPP

#include "PyObject.hpp"
#include "FloatKlass.hpp"  

class PyFloat : public PyObject {
    friend class FloatKlass;

private:
    double value;

public:
    PyFloat(double v);

    static PyFloat* createFloat(double value);

    double getValue() const { return value; }

    PyObject* add(PyObject* other);
    PyObject* sub(PyObject* other);
    PyObject* mul(PyObject* other);
    PyObject* div(PyObject* other);

    void* operator new(size_t size);
    void operator delete(void* p);
};

#endif
