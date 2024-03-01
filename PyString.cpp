#include "PyString.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include "StringKlass.hpp"

PyString::PyString(char* str_source) {
    length = strlen(str_source);
    ptr = new uint8_t[length];
    memcpy(ptr, str_source, sizeof(uint8_t) * length);
    setKlass(StringKlass::getInstance());
}

PyString::PyString(uint8_t* source, size_t len_src) {
    length = len_src;
    ptr = new uint8_t[len_src];
    memcpy(ptr, source, sizeof(uint8_t) * len_src);
    setKlass(StringKlass::getInstance());
}

PyString::~PyString() {
    delete[] ptr;
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
