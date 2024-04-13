#ifndef String_Klass_Hpp
#define String_Klass_Hpp

#include "Klass.hpp"

class StringKlass : public Klass {
private:
    StringKlass() {};
    static StringKlass* instance;
public:
    static StringKlass* getInstance() {
        if (instance == nullptr) {
            instance = new StringKlass();
        }
        return instance;
    }

    virtual void print(const PyObject* lhs) const override;

    // 各种比大小
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const override;

    virtual PyObject* len(const PyObject* strObject) const override;
    virtual PyObject* subscr(PyObject* strObject, PyObject* subscription) const override;
    virtual PyObject* has(PyObject* object, PyObject* target) const override;
};

#endif