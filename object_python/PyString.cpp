#include "PyString.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include "StringKlass.hpp"
#include "Universe.hpp"

PyString::PyString(const char* str_source) :
    PyString(
        reinterpret_cast<const uint8_t*>(str_source), 
        strlen(str_source)
    )
{

}

PyString::PyString(const uint8_t* source, size_t len_src) {
    length = len_src;
    ptr = reinterpret_cast<uint8_t*>(Universe::PyHeap->allocate(length + 1));
    memcpy(ptr, source, sizeof(uint8_t) * length);
    ptr[length] = 0;
    setKlass(StringKlass::getInstance());
}

PyString::~PyString() {
    delete[] ptr;
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
