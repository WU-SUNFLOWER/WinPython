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

    virtual void print(const PyObject* lhs) const override;

    // ���ֱȴ�С
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const override;

    // ��������
    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* sub(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* div(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mod(const PyObject* lhs, const PyObject* rhs) const;
};

#endif