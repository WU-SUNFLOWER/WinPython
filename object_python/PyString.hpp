#ifndef PyString_Hpp
#define PyString_Hpp

#include "PyObject.hpp"
#include <cstdint>

class PyString : public PyObject {
    friend class StringKlass;
private:
    uint8_t* ptr;
    size_t length;
public:
    PyString(const char* str_source);
    PyString(const uint8_t* source, size_t length);
    ~PyString();

    const uint8_t* getValue() const {
        return ptr;
    }
    
    size_t getLength() const {
        return length;
    }
    
    uint8_t operator [](size_t index);
};
#endif
