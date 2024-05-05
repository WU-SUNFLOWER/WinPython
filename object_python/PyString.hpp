#ifndef PyString_Hpp
#define PyString_Hpp

#include "PyObject.hpp"
#include <cstdint>

class PyString : public PyObject {
    friend class StringKlass;
private:
    uint8_t* ptr = nullptr;
    size_t length = 0;
    PyString() {};
public:
    static PyString* createString(const char* str_source, bool isMeta = false);
    static PyString* createString(const uint8_t* source, size_t len_src, bool isMeta = false);

    const uint8_t* getValue() const {
        return ptr;
    }
    
    size_t getLength() const {
        return length;
    }
    
    uint8_t operator [](size_t index);
};
#endif
