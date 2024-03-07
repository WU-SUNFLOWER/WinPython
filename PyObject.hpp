#ifndef PyObject_Hpp
#define PyObject_Hpp

#include "arrayList.hpp"
#include "map.hpp"
#include "Klass.hpp"
#include <cassert>

class PyObject {
private:
    // 不同类型的python对象通过klass指针的指向不同来区分
    Klass* klass;
public:

    PyObject() : klass(nullptr) {};

    void setKlass(Klass* k) {
        klass = k;
    }
    const Klass* getKlass() const {
        assert(klass != nullptr);
        return klass;
    }

    /*公用方法 Start*/

    void print() const;

    PyObject* add(PyObject* other) const;
    PyObject* sub(PyObject* other) const;
    PyObject* mul(PyObject* other) const;
    PyObject* div(PyObject* other) const;
    PyObject* mod(PyObject* other) const;

    PyObject* less(PyObject* other) const;
    PyObject* less_equal(PyObject* other) const;
    PyObject* equal(PyObject* other) const;
    PyObject* greater_equal(PyObject* other) const;
    PyObject* greater(PyObject* other) const;
    PyObject* not_equal(PyObject* other) const;

    // 计算长度
    PyObject* len() const;
    // 获取对象属性
    PyObject* getattr(PyObject* attr) const;

    /*公用方法 End*/
};

typedef Map<PyObject*, PyObject*> PyObjectMap;
#endif