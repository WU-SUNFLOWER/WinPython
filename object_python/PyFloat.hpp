#ifndef PyFloat_Hpp
#define PyFloat_Hpp

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


    void* operator new(size_t size);
    void operator delete(void* p);
};

#endif
