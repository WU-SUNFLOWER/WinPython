#ifndef Klass_Hpp
#define Klass_Hpp

#include "map.hpp"

class PyObject;

class Klass {
private:
    Map<PyObject*, PyObject*>* _klassDict;

protected:
    Klass() {
        _klassDict = new Map<PyObject*, PyObject*>();
    }
public:

    virtual void print(const PyObject* lhs) const {};

    static void checkLegalPyObject(const PyObject* obj, const Klass* klass);

    void setKlassDict(Map<PyObject*, PyObject*>* dict) {
        _klassDict = dict;
    };
    Map<PyObject*, PyObject*>* getKlassDict() const {
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
};

#define checkLegalPyObject_DB(x, y) (Klass::checkLegalPyObject(x, this), Klass::checkLegalPyObject(y, this))

#endif