#ifndef PyString_Hpp
#define PyString_Hpp

#include "PyObject.hpp"
#include <cstdint>

class PyString : public PyObject {
    friend class StringKlass;
private:
    size_t length = 0;
    PyString() {};
public:
    uint8_t* ptr = nullptr;
    static PyString* createString(const char* str_source);
    static PyString* createString(const uint8_t* source, size_t len_src);

    const uint8_t* getValue() const {
        return ptr;
    }
    
    size_t getLength() const {
        return length;
    }
    
    uint8_t operator [](size_t index);
};
#endif
