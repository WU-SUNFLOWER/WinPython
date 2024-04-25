#ifndef PyMethod_Hpp
#define PyMethod_Hpp

#include "PyObject.hpp"
#include "PyFunction.hpp"
#include "MethodKlass.hpp"

class PyMethod : public PyObject {
private:
    PyObject* _owner;
    PyFunction* _func;
public:
    PyMethod(PyFunction* func, PyObject* owner) 
        : _owner(owner), _func(func) {
        setKlass(MethodKlass::getInstance());
    }

    PyObject* getOwner() const {
        return _owner;
    }
    PyFunction* getFunc() const {
        return _func;
    }
};

#endif