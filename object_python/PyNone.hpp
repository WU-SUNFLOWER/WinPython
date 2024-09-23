#ifndef PYNONE_HPP
#define PYNONE_HPP

#include "PyObject.hpp"

class PyNone : public PyObject {
public:
    PyNone();

    static PyNone* createNone();
};

#endif
