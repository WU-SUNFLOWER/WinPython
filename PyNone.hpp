#ifndef PYNONE_HPP
#define PYNONE_HPP

#include "PyObject.hpp"

class PyNone : public PyObject {
public:
    PyNone();

    static PyNone* createNone();

    void* operator new(size_t size);
    void operator delete(void* p);
};

#endif
