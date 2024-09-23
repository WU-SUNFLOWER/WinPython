#ifndef PyObject_Hpp
#define PyObject_Hpp

#include "arrayList.hpp"
#include "map.hpp"
#include "Klass.hpp"
#include <cassert>
#include <cstdio>
#include "OopClosure.hpp"

class PyDict;

class PyObject {
private:
    uintptr_t _mark_word;
    // 不同类型的python对象通过klass指针的指向不同来区分
    Klass* klass;

    // 用来保存定义在PyObject实例之上的属性
    PyDict* _self_dict;
public:

    PyObject();

    void setKlass(Klass* k) {
        klass = k;
    }
    Klass* getKlass() const {
        assert(klass != nullptr);
        return klass;
    }
    PyString* getKlassName() const;
    const uint8_t* getKlassNameAsString() const;

    PyDict* getSelfDict();
    void setSelfDict(Handle<PyDict*> dict);
    PyDict* initSelfDict();

    void* operator new(size_t size);
    void operator delete(void*);

    /*公用方法 Start*/

    void print(int flags = 0) const;

    PyObject* add(PyObject* other) const;
    PyObject* sub(PyObject* other) const;
    PyObject* mul(PyObject* other) const;
    PyObject* div(PyObject* other) const;
    PyObject* mod(PyObject* other) const;
    PyObject* floor_div(PyObject* other) const;

    PyObject* inplace_add(PyObject* other);

    PyObject* less(const PyObject* other) const;
    PyObject* less_equal(const PyObject* other) const;
    PyObject* equal(const PyObject* other) const;
    PyObject* greater_equal(const PyObject* other) const;
    PyObject* greater(const PyObject* other) const;
    PyObject* not_equal(const PyObject* other) const;

    PyObject* isBoolTrue();

    // 计算长度
    PyObject* len() const;
    // 获取对象属性
    PyObject* getattr(PyObject* attr);
    // 设置对象属性
    void setattr(PyObject* attr, PyObject* value);
    // 对象取下标
    PyObject* subscr(PyObject* subscription);
    // 对象下标赋新值
    void store_subscr(PyObject* subscription, PyObject* newObject);
    // 对象删除下标对应元素
    void delete_subscr(PyObject* subscription);
    // 检查对象是否包含某些东西
    PyObject* has(PyObject* target);

    // 迭代器相关
    PyObject* getIter();
    PyObject* next();

    PyObject* isinstance(PyTypeObject* type) const;

    /*公用方法 End*/
    template<class T>
    inline T* as();

    // GC相关接口
    void* getNewAddr();
    void setNewAddr(void*);
    size_t getSize();
    void oops_do(OopClosure* closure);
};

typedef Map<PyObject*, PyObject*> PyObjectMap;
#endif