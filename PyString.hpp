#ifndef PyString_Hpp
#define PyString_Hpp

#include "PyObject.hpp"
#include <cstdint>

class PyString : public PyObject {
private:
    uint8_t* ptr;
    size_t length;
public:
    PyString(char* str_source);
    PyString(uint8_t* source, size_t length);
    ~PyString();
    virtual void print() const;
    virtual PyObject* add(PyObject* other) const override;
    const uint8_t* getValue() const {
        return ptr;
    }
    size_t getLength() const {
        return length;
    }
    uint8_t operator [](size_t index);
};
#endif
