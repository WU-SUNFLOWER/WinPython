#include "PySuper.hpp"

PySuper::PySuper(PyTypeObject* type, PyObject* object)
    : object(object), type(type) {
    this->object_type = static_cast<PyTypeObject*>(object->getKlass()->getTypeObject());
}
