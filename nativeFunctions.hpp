#ifndef NativeFunction_Hpp
#define NativeFunction_Hpp

#include "PyObject.hpp"
#include "PyList.hpp"

class NativeFunction {
public:
    static PyObject* len(PyList* args);
    static PyObject* string_upper(PyList* args);
};

#endif