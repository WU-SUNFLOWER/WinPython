#ifndef PyObject_Hpp
#define PyObject_Hpp

#include "arrayList.hpp"
#include "map.hpp"
#include "Klass.hpp"
#include <cassert>

class PyDict;

class PyObject {
private:
    // ��ͬ���͵�python����ͨ��klassָ���ָ��ͬ������
    Klass* klass;

    // �������涨����PyObjectʵ��֮�ϵ�����
    PyDict* _self_dict;
public:

    PyObject() : klass(nullptr), _self_dict(nullptr) {};

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
    PyDict* initSelfDict();

    /*���÷��� Start*/

    void print(int flags = 0) const;

    PyObject* add(PyObject* other) const;
    PyObject* sub(PyObject* other) const;
    PyObject* mul(PyObject* other) const;
    PyObject* div(PyObject* other) const;
    PyObject* mod(PyObject* other) const;

    PyObject* inplace_add(PyObject* other);

    PyObject* less(const PyObject* other) const;
    PyObject* less_equal(const PyObject* other) const;
    PyObject* equal(const PyObject* other) const;
    PyObject* greater_equal(const PyObject* other) const;
    PyObject* greater(const PyObject* other) const;
    PyObject* not_equal(const PyObject* other) const;

    // ���㳤��
    PyObject* len() const;
    // ��ȡ��������
    PyObject* getattr(PyObject* attr);
    // ���ö�������
    void setattr(PyObject* attr, PyObject* value);
    // ����ȡ�±�
    PyObject* subscr(PyObject* subscription);
    // �����±긳��ֵ
    void store_subscr(PyObject* subscription, PyObject* newObject);
    // ����ɾ���±��ӦԪ��
    void delete_subscr(PyObject* subscription);
    // �������Ƿ����ĳЩ����
    PyObject* has(PyObject* target);

    // ���������
    PyObject* getIter();
    PyObject* next();

    /*���÷��� End*/
};

typedef Map<PyObject*, PyObject*> PyObjectMap;
#endif