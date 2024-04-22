#ifndef Hpp_PyTypeObject
#define Hpp_PyTypeObject

#include "Klass.hpp"
#include "PyObject.hpp"

class PyTypeObject:public PyObject {
private:
    Klass* own_klass;
public:
    PyTypeObject();
    void setOwnKlass(Klass* klass);
    Klass* getOwnKlass() const;
};

#endif