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

    PyObject* less(const PyObject* other) const;
    PyObject* less_equal(const PyObject* other) const;
    PyObject* equal(const PyObject* other) const;
    PyObject* greater_equal(const PyObject* other) const;
    PyObject* greater(const PyObject* other) const;
    PyObject* not_equal(const PyObject* other) const;

    // 计算长度
    PyObject* len() const;
    // 获取对象属性
    PyObject* getattr(PyObject* attr) const;
    // 对象取下标
    PyObject* subscr(PyObject* subscription);
    // 对象下标赋新值
    PyObject* store_subscr(PyObject* subscription, PyObject* newObject);
    // 对象删除下标对应元素
    void delete_subscr(PyObject* subscription);
    // 检查对象是否包含某些东西
    PyObject* has(PyObject* target);

    // 迭代器相关
    PyObject* getIter();
    PyObject* next();

    /*公用方法 End*/
};

typedef Map<PyObject*, PyObject*> PyObjectMap;
#endif