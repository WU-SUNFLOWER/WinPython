#include "PyString.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include "StringKlass.hpp"
#include "Universe.hpp"

PyString* PyString::createString(
    const uint8_t* source, size_t len_src
) {

    Handle<PyString*> str = new PyString();
    str->length = len_src;
    str->setKlass(StringKlass::getInstance());
    
    uint8_t* ptr = reinterpret_cast<uint8_t*>(Universe::PyHeap->allocate(len_src + 1));
    memcpy(ptr, source, sizeof(uint8_t) * len_src);
    ptr[len_src] = 0;
    str->ptr = ptr;

    return str;
}

PyString* PyString::createString(const char* str_source) {
    return createString(
        reinterpret_cast<const uint8_t*>(str_source),
        strlen(str_source));
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
