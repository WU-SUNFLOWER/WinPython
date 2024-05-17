#ifndef PYBOOL_HPP
#define PYBOOL_HPP

#include "PyObject.hpp"

class PyBool : public PyObject {
private:
    bool value;

public:
    PyBool(bool v);

    static PyBool* createBool(bool value);

    bool getValue() const { return value; }

    void* operator new(size_t size);
    void operator delete(void* p);
};

#endif
