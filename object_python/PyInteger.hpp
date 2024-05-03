#ifndef PyInteger_Hpp
#define PyInteger_Hpp

#include "IntegerKlass.hpp"
#include "PyObject.hpp"
#include <cstdint>

class PyInteger: public PyObject {
    friend class IntegerKlass;
private:
    int64_t value;
public:
    PyInteger(int64_t v) : value(v) {
        setKlass(IntegerKlass::getInstance());
    };
    
    int64_t getValue() const {
        return value;
    };
    
};
#endif