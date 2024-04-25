#ifndef Hpp_TypeKlass
#define Hpp_TypeKlass

#include "Klass.hpp"

class TypeKlass : public Klass {
private:
    TypeKlass() {};
    static TypeKlass* instance;
public:
    static TypeKlass* getInstance();
    virtual void print(const PyObject* object, int flag) const override;
    void initialize() override;

    virtual PyObject* getattr(PyObject* object, PyObject* attr) override;
    virtual void setattr(PyObject* object, PyObject* attr, PyObject* value) override;

    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const override;
};

#endif