#include "PyString.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include "StringKlass.hpp"
#include "Universe.hpp"

PyString* PyString::createString(
    const uint8_t* source, size_t len_src, bool isMeta
) {
    START_COUNT_TEMP_OBJECTS;

    PyString* str = new (isMeta)PyString();
    str->length = len_src;
    str->setKlass(StringKlass::getInstance());
    str->isInMeta = isMeta;
    PUSH_TEMP(str);
    
    uint8_t* ptr = nullptr;
    if (isMeta) {
        ptr = reinterpret_cast<uint8_t*>(Universe::PyHeap->allocateMeta(len_src + 1));
    }
    else {
        ptr = reinterpret_cast<uint8_t*>(Universe::PyHeap->allocate(len_src + 1));
    }
    memcpy(ptr, source, sizeof(uint8_t) * len_src);
    ptr[len_src] = 0;
    str->ptr = ptr;

    END_COUNT_TEMP_OBJECTS;
    return str;
}

PyString* PyString::createString(const char* str_source, bool isMeta) {
    return createString(
        reinterpret_cast<const uint8_t*>(str_source),
        strlen(str_source),
        isMeta);
}

uint8_t PyString::operator[](size_t index) {
    assert(index >= 0 && index < getLength());
    return ptr[index];
}
