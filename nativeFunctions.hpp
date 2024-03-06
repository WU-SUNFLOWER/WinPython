#ifndef NativeFunction_Hpp
#define NativeFunction_Hpp

#include "PyObject.hpp"

class NativeFunction {
public:
    static PyObject* len(PyObjectList* args);
    static PyObject* string_upper(PyObjectList* args);
};

#endif