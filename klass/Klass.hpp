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

    Klass* _super = nullptr; // ָ��ǰklass�ĸ��ࣨ���ࣩ

    PyTypeObject* _type_object = nullptr;  // ָ��ǰklass��Ӧ��PyTypeObject����

protected:
    Klass();
public:
    // ��ȡPython�����ռ�õĿռ��С
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
    virtual PyObject* inplace_add(PyObject* lhs, PyObject* rhs);

    // �������ߺ���
    virtual PyObject* len(const PyObject* object) const { return 0; };
    // ��ȡ����
    virtual PyObject* getattr(PyObject* object, PyObject* attr);
    // ��������
    virtual void setattr(PyObject* object, PyObject* attr, PyObject* value);
    // ��ȡ�±�
    virtual PyObject* subscr(PyObject* object,
        PyObject* subscrption) const { return 0; }
    // �����±�
    virtual void store_subscr(PyObject* object, 
        PyObject* subscription, PyObject* newObject) const { return; }
    virtual void delete_subscr(PyObject* object,
        PyObject* subscription) const {return;}
    virtual PyObject* has(PyObject* object, PyObject* target) const { return 0; }

    // ���������
    virtual PyObject* getIter(PyObject* object) const { return 0; }
    virtual PyObject* next(PyObject* object) const { return 0; }

    // ���ʵ����
    virtual PyObject* allocateInstance(PyObject* callable, PyList* args);

    // GC��ؽӿ�
    virtual void oops_do(OopClosure* closure);
    virtual void oops_do(OopClosure* closure, PyObject* object);
};

#define checkLegalPyObject(obj, klass) assert(obj != nullptr && obj->getKlass() == klass)
#define checkLegalPyObject_DB(x, y) { \
    checkLegalPyObject(x, this); \
    checkLegalPyObject(y, this); \
}

#endif