#ifndef Klass_Hpp
#define Klass_Hpp

#include "map.hpp"

class PyObject;
class PyDict;

class Klass {
private:
    PyDict* _klassDict = nullptr;
protected:
    Klass() {};
public:

    virtual void initialize() { return; };

    virtual void print(const PyObject* lhs) const {};

    static void checkLegalPyObject(const PyObject* obj, const Klass* klass);

    void setKlassDict(PyDict* dict) {
        _klassDict = dict;
    };
    PyDict* getKlassDict() const {
        return _klassDict;
    }

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

    // 其他工具函数
    virtual PyObject* len(const PyObject* object) const { return 0; };
    virtual PyObject* subscr(PyObject* object,
        PyObject* subscrption) const { return 0; }
    virtual PyObject* store_subscr(PyObject* object, 
        PyObject* subscription, PyObject* newObject) const { return 0; }
    virtual void delete_subscr(PyObject* object,
        PyObject* subscription) const {return;}
    virtual PyObject* has(PyObject* object, PyObject* target) const { return 0; }

    // 迭代器相关
    virtual PyObject* getIter(PyObject* object) const { return 0; }
    virtual PyObject* next(PyObject* object) const { return 0; }
};

#define checkLegalPyObject_DB(x, y) (Klass::checkLegalPyObject(x, this), Klass::checkLegalPyObject(y, this))

#endif