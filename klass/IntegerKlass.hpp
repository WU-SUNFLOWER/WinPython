#ifndef Integer_Klass_Hpp
#define Integer_Klass_Hpp

#include "Klass.hpp"

class IntegerKlass : public Klass {
private:
    IntegerKlass() {};
    static IntegerKlass* instance;
public:
    static IntegerKlass* getInstance() {
        if (instance == nullptr) {
            instance = new IntegerKlass();
        }
        return instance;
    }

    virtual void initialize() override;

    virtual void print(const PyObject* lhs, int flags) const override;

    // 各种比大小
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const override;

    // 各种运算
    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* sub(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* div(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mod(const PyObject* lhs, const PyObject* rhs) const;
    virtual PyObject* inplace_add(PyObject* lhs, PyObject* rhs) override;

    // 类的实例化
    virtual PyObject* allocateInstance(PyObject* callable, PyList* args) override;
};

#endif