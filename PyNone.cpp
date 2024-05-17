#include "PyNone.hpp"
#include "Universe.hpp"
#include "NoneKlass.hpp"

PyNone* PyNone::createNone() {
    return new PyNone();
}

void* PyNone::operator new(size_t size) {
    return Universe::PyHeap->allocateMeta(size);
}

void PyNone::operator delete(void*) {
    puts("Can't delete PyNone object.");
    exit(-1);
}

PyNone::PyNone() {
    setKlass(NoneKlass::getInstance());
}
