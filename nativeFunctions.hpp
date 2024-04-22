#ifndef NativeFunction_Hpp
#define NativeFunction_Hpp

#include "PyObject.hpp"
#include "PyList.hpp"
#include "PyFunction.hpp"

class NativeFunction {
public:
    static PyObject* len(PyList* args);
    static PyObject* isinstance(PyList* args);
    static PyObject* type_of(PyList* args);

    static PyObject* string_upper(PyList* args);

    static PyObject* list_append(PyList* args);
    static PyObject* list_insert(PyList* args);
    static PyObject* list_index(PyList* args);
    static PyObject* list_pop(PyList* args);
    static PyObject* list_remove(PyList* args);
    static PyObject* list_reverse(PyList* args);
    static PyObject* list_iterator_next(PyList* args);

    static PyObject* dict_set_default(PyList* args);
    static PyObject* dict_pop(PyList* args);
    static PyObject* dict_keys(PyList* args);
    static PyObject* dict_values(PyList* args);
    static PyObject* dict_items(PyList* args);
};

#define PackNativeFunc(f) (new PyFunction(reinterpret_cast<NativeFuncPointer>(f)))

#endif