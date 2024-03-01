#ifndef PyInteger_Hpp
#define PyInteger_Hpp

#include "IntegerKlass.hpp"
#include "PyObject.hpp"
#include <cstdint>

class PyInteger: public PyObject {
private:
    int32_t value;
public:
    PyInteger(int32_t v) : value(v) {
        setKlass(IntegerKlass::getInstance());
    };
    
    int32_t getValue() const {
        return value;
    };
    
};
#endif