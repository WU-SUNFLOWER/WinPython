#include "PyString.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include "StringKlass.hpp"

PyString::PyString(const char* str_source) {
    length = strlen(str_source);
    ptr = new uint8_t[length + 1];
    memcpy(ptr, str_source, sizeof(uint8_t) * length);
    ptr[length] = 0;  // 人工添加一个结束符
    setKlass(StringKlass::getInstance());
}

PyString::PyString(const uint8_t* source, size_t len_src) {
    length = len_src;
    ptr = new uint8_t[len_src + 1];
    memcpy(ptr, source, sizeof(uint8_t) * len_src);
    ptr[len_src] = 0;
    setKlass(StringKlass::getInstance());
}

PyString::~PyString() {
    delete[] ptr;
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
