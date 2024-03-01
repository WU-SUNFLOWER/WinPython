#ifndef Klass_Hpp
#define Klass_Hpp

class PyObject;

class Klass {
private:
public:

    virtual void print(const PyObject* lhs) const {};

    static void checkLegalPyObject(const PyObject* obj, const Klass* klass);

    // 各种比大小
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };

    // 各种运算
    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* sub(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* div(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* mod(const PyObject* lhs, const PyObject* rhs) const { return 0; };
};

#define checkLegalPyObject_DB(x, y) (Klass::checkLegalPyObject(x, this), Klass::checkLegalPyObject(y, this))

#endif