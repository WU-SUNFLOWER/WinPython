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

    PyObject* less(PyObject* other) const;
    PyObject* less_equal(PyObject* other) const;
    PyObject* equal(PyObject* other) const;
    PyObject* greater_equal(PyObject* other) const;
    PyObject* greater(PyObject* other) const;
    PyObject* not_equal(PyObject* other) const;

    // ���㳤��
    PyObject* len() const;
    // ��ȡ��������
    PyObject* getattr(PyObject* attr) const;

    /*���÷��� End*/
};

typedef Map<PyObject*, PyObject*> PyObjectMap;
#endif