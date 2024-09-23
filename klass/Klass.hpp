#ifndef Klass_Hpp
#define Klass_Hpp

#include "OopClosure.hpp"
#include "map.hpp"
#include "Handle.hpp"

class PyObject;
class PyString;
class PyList;
class PyDict;
class PyTypeObject;

class Klass {
private:
    PyDict* _klassDict = nullptr;

    PyString* _name = nullptr;

    PyList* _mro = nullptr;
    PyList* _supers = nullptr;  // 父类（type object）列表

    PyTypeObject* _type_object = nullptr;  // 指向当前klass对应的PyTypeObject对象

protected:
    Klass();
    PyObject* find_in_parents(PyObject* x, PyObject* y);
public:
    // 获取Python对象会占用的空间大小
    virtual size_t getSize();

    static PyObject* createKlass(Handle<PyObject*> dict, Handle<PyObject*> supers, Handle<PyObject*> name);

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
        assert(_type_object != nullptr);
        return reinterpret_cast<PyObject*>(_type_object);
    }

    void setName(PyString* name) {
        _name = name;
    }
    PyString* getName() const {
        return _name;
    }

    PyList* mro() {
        return _mro;
    }

    void setSuperList(PyList* supers) {
        _supers = supers;
    }

    void addSuper(Klass* x);

    void orderSupers();

    PyList* getSuperKlass() const {
        return _supers;
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
    virtual PyObject* floor_div(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* inplace_add(PyObject* lhs, PyObject* rhs);

    // 其他工具函数
    virtual PyDict* init_self_dict(Handle<PyObject*> object);
    virtual PyObject* len(const PyObject* object) const { return 0; };
    // 读取属性
    virtual PyObject* getattr(Handle<PyObject*> object, Handle<PyObject*> attr);
    // 设置属性
    virtual void setattr(Handle<PyObject*> object, Handle<PyObject*> attr, Handle<PyObject*> value);
    // 读取下标
    virtual PyObject* subscr(PyObject* object,
        PyObject* subscrption) const {
        return 0;
    }
    // 设置下标
    virtual void store_subscr(PyObject* object,
        PyObject* subscription, PyObject* newObject) const {
        return;
    }
    virtual void delete_subscr(PyObject* object,
        PyObject* subscription) const {
        return;
    }
    virtual PyObject* has(PyObject* object, PyObject* target) const { return 0; }

    // 迭代器相关
    virtual PyObject* getIter(PyObject* object) const { return 0; }
    virtual PyObject* next(PyObject* object) const { return 0; }

    // 类的实例化
    virtual PyObject* allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args);

    // GC相关接口
    virtual void oops_do(OopClosure* closure);
    virtual void oops_do(OopClosure* closure, PyObject* object);

    virtual PyObject* isBoolTrue(PyObject* object);

};

#define checkLegalPyObject(obj, klass) assert(obj != nullptr && obj->getKlass() == klass)
#define checkLegalPyObject_DB(x, y) { \
    checkLegalPyObject(x, this); \
    checkLegalPyObject(y, this); \
}

#endif