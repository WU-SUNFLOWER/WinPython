#include "PyBool.hpp"
#include "Universe.hpp"
#include "BoolKlass.hpp"

PyBool* PyBool::createBool(bool value) {
    PyBool* obj = new PyBool(value);
    obj->setKlass(BoolKlass::getInstance());
    return  obj;
}

void* PyBool::operator new(size_t size) {
    return Universe::PyHeap->allocateMeta(size);
}

void PyBool::operator delete(void*) {
    puts("Can't delete PyBool object.");
    exit(-1);
}

PyBool::PyBool(bool v) : value(v) {
    
}
