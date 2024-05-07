#include "CodeObject.hpp"
#include "Universe.hpp"

void* CodeObject::operator new(size_t size) {
    return Universe::PyHeap->allocateMeta(size);
}

void CodeObject::operator delete(void*) {
    puts("Can't delete code object.");
    exit(-1);
}