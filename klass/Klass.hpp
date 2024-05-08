#ifndef Klass_Hpp
#define Klass_Hpp

#include "OopClosure.hpp"
#include "map.hpp"

class PyObject;
class PyString;
class PyList;
class PyDict;
class PyTypeObject;

class Klass {
private:
    PyDict* _klassDict = nullptr;

    PyString* _name = nullptr;

    Klass* _super = nullptr; // 指向当前klass的父类（基类）

    PyTypeObject* _type_object = nullptr;  // 指向当前klass对应的PyTypeObject对象

protected:
    Klass();
public:
    // 获取Python对象会占用的空间大小
    virtual size_t getSize();

    static PyObject* createKlass(PyObject* dict, PyObject* supers, PyObject* name);

    virtual void initialize() { return; };

    void* operator new(size_t size);

#define FLAG_PyString_PRINT_RAW 1
    virtual void print(const PyObject* lhs, int flags = 0) const;

    //static void checkLegalPyObject(const PyObject* obj, const Klass* klass);

    void setKlassDict(PyDict* dict) {
        _klassDict = dict;
    };
    PyDict* getKlassDict() const {
        return _klassDict;
    }

    void setTypeObject(PyTypeObject* object) {
        _type_object = object;
    }
    PyObject* getTypeObject() {
        return reinterpret_cast<PyObject*>(_type_object);
    }

    void setName(PyString* name) {
        _name = name;
    }
    PyString* getName() const {
        return _name;
    }

    void setSuperKlass(Klass* super) {
        _super = super;
    }
    Klass* getSuperKlass() const {
        return _super;
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
    virtual PyObject* inplace_add(PyObject* lhs, PyObject* rhs);

    // 其他工具函数
    virtual PyObject* len(const PyObject* object) const { return 0; };
    // 读取属性
    virtual PyObject* getattr(PyObject* object, PyObject* attr);
    // 设置属性
    virtual void setattr(PyObject* object, PyObject* attr, PyObject* value);
    // 读取下标
    virtual PyObject* subscr(PyObject* object,
        PyObject* subscrption) const { return 0; }
    // 设置下标
    virtual void store_subscr(PyObject* object, 
        PyObject* subscription, PyObject* newObject) const { return; }
    virtual void delete_subscr(PyObject* object,
        PyObject* subscription) const {return;}
    virtual PyObject* has(PyObject* object, PyObject* target) const { return 0; }

    // 迭代器相关
    virtual PyObject* getIter(PyObject* object) const { return 0; }
    virtual PyObject* next(PyObject* object) const { return 0; }

    // 类的实例化
    virtual PyObject* allocateInstance(PyObject* callable, PyList* args);

    // GC相关接口
    virtual void oops_do(OopClosure* closure);
    virtual void oops_do(OopClosure* closure, PyObject* object);
};

#define checkLegalPyObject(obj, klass) assert(obj != nullptr && obj->getKlass() == klass)
#define checkLegalPyObject_DB(x, y) { \
    checkLegalPyObject(x, this); \
    checkLegalPyObject(y, this); \
}

#endif