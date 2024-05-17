#ifndef FLOATKLASS_HPP
#define FLOATKLASS_HPP

#include "Klass.hpp"
class PyFloat;

class FloatKlass : public Klass {
private:
    static FloatKlass* instance;

    FloatKlass() {}

public:
    static FloatKlass* getInstance();
    
    virtual void initialize() override;

    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* sub(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* div(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* floor_div(const PyObject* lhs, const PyObject* rhs) const override;

    virtual void print(const PyObject* obj, int flags) const override;

    virtual PyObject* isBoolTrue(PyObject* object) override;
};

#endif
