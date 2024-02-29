#include "PyString.hpp"

PyString::PyString(char* str_source) {
    length = strlen(str_source);
    ptr = new uint8_t[length];
    memcpy(ptr, str_source, sizeof(uint8_t) * length);
}

PyString::PyString(uint8_t* source, size_t len_src) {
    length = len_src;
    ptr = new uint8_t[len_src];
    memcpy(ptr, source, sizeof(uint8_t) * len_src);
}

PyString::~PyString() {
    delete[] ptr;
}

void PyString::print() const {
    for (size_t i = 0; i < length; ++i) {
        putchar(static_cast<char>(ptr[i]));
    }
}

PyObject* PyString::add(PyObject* other) const {
    return nullptr;
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
