#ifndef PySuper_Hpp

#include "PyObject.hpp"
#include "PyTypeObject.hpp"

class PySuper: public PyObject {
    friend class SuperKlass;
private:
    PyObject* object;
    PyTypeObject* type;
    PyTypeObject* object_type;
public:
    PySuper(PyTypeObject* type, PyObject* object);
};

#endif