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
    PyList* _supers = nullptr;  // ���ࣨtype object���б�

    PyTypeObject* _type_object = nullptr;  // ָ��ǰklass��Ӧ��PyTypeObject����

protected:
    Klass();
    PyObject* find_in_parents(PyObject* x, PyObject* y);
public:
    // ��ȡPython�����ռ�õĿռ��С
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

    // ���ֱȴ�С
    virtual PyObject* less(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* less_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* greater_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* greater(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* not_equal(const PyObject* lhs, const PyObject* rhs) const { return 0; };

    // ��������
    virtual PyObject* add(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* sub(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* mul(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* div(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* mod(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* floor_div(const PyObject* lhs, const PyObject* rhs) const { return 0; };
    virtual PyObject* inplace_add(PyObject* lhs, PyObject* rhs);

    // �������ߺ���
    virtual PyDict* init_self_dict(Handle<PyObject*> object);
    virtual PyObject* len(const PyObject* object) const { return 0; };
    // ��ȡ����
    virtual PyObject* getattr(Handle<PyObject*> object, Handle<PyObject*> attr);
    // ��������
    virtual void setattr(Handle<PyObject*> object, Handle<PyObject*> attr, Handle<PyObject*> value);
    // ��ȡ�±�
    virtual PyObject* subscr(PyObject* object,
        PyObject* subscrption) const {
        return 0;
    }
    // �����±�
    virtual void store_subscr(PyObject* object,
        PyObject* subscription, PyObject* newObject) const {
        return;
    }
    virtual void delete_subscr(PyObject* object,
        PyObject* subscription) const {
        return;
    }
    virtual PyObject* has(PyObject* object, PyObject* target) const { return 0; }

    // ���������
    virtual PyObject* getIter(PyObject* object) const { return 0; }
    virtual PyObject* next(PyObject* object) const { return 0; }

    // ���ʵ����
    virtual PyObject* allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args);

    // GC��ؽӿ�
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