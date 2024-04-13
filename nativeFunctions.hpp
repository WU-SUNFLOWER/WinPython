#ifndef NativeFunction_Hpp
#define NativeFunction_Hpp

#include "PyObject.hpp"
#include "PyList.hpp"

class NativeFunction {
public:
    static PyObject* len(PyList* args);
    static PyObject* string_upper(PyList* args);
    static PyObject* list_append(PyList* args);
    static PyObject* list_insert(PyList* args);
    static PyObject* list_index(PyList* args);
    static PyObject* list_pop(PyList* args);
    static PyObject* list_remove(PyList* args);
    static PyObject* list_reverse(PyList* args);
};

#endif