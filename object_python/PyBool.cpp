#include "PyBool.hpp"
#include "Universe.hpp"
#include "BoolKlass.hpp"

PyBool* PyBool::createBool(bool value) {
    Handle<PyBool*> obj = new PyBool(value);
    obj->setKlass(BoolKlass::getInstance());
    return obj;
}

PyBool::PyBool(bool v) : value(v) {
    
}
