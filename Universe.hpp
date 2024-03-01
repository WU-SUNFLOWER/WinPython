#ifndef Universe_Hpp
#define Universe_Hpp

#include "PyObject.hpp"

namespace Universe {
    extern PyObject* PyTrue;
    extern PyObject* PyFalse;
    extern PyObject* PyNone;
    void genesis();
    void destroy();
}

#endif