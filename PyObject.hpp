#ifndef PyObject_Hpp
#define PyObject_Hpp

#include "arrayList.hpp"
#include "map.hpp"
#include "Klass.hpp"
#include <cassert>

class PyObject {
private:
    // ��ͬ���͵�python����ͨ��klassָ���ָ��ͬ������
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

    /*���÷��� Start*/

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

    // ���㳤��
    PyObject* len() const;
    // ��ȡ��������
    PyObject* getattr(PyObject* attr) const;
    // ����ȡ�±�
    PyObject* subscr(PyObject* subscription);
    // �����±긳��ֵ
    PyObject* store_subscr(PyObject* subscription, PyObject* newObject);
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