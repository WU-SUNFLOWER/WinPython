#ifndef ListKlass_Hpp
#define ListKlass_Hpp

#include "Klass.hpp"
#include "PyList.hpp"

class ListKlass : public Klass {
private:
    ListKlass() {};
    static ListKlass* instance;
public:
    static ListKlass* getInstance() {
        if (instance == nullptr) {
            instance = new ListKlass();
        }
        return instance;
    }
    virtual void print(const PyObject* object) const override;

    // 各种比大小
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const override;

    // 各种运算
    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const override;
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const override;

    virtual PyObject* subscr(PyObject* object, 
        PyObject* subscrption) const override;
    virtual PyObject* store_subscr(PyObject* object,
        PyObject* subscription, PyObject* newObject) const override;
    virtual PyObject* delete_subscr(PyObject* object,
        PyObject* subscription) const override;
    virtual PyObject* has(PyObject* object, PyObject* target) const override;
};

#endif