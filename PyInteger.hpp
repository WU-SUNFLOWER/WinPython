#ifndef PyInteger_Hpp
#define PyInteger_Hpp

#include "PyObject.hpp"
#include <cstdint>

class PyInteger: public PyObject {
private:
    int32_t value;
public:
    PyInteger(int32_t v) : value(v) {};
    
    int32_t getValue() const;
    
    virtual void print() const;
    
    virtual PyObject* add(PyObject* other) const override;
    
    virtual PyObject* less(PyObject* other) const override;
    virtual PyObject* less_equal(PyObject* other) const override;
    virtual PyObject* equal(PyObject* other) const override;
    virtual PyObject* greater_equal(PyObject* other) const override;
    virtual PyObject* greater(PyObject* other) const override;
    virtual PyObject* not_equal(PyObject* other) const override;
};
#endif