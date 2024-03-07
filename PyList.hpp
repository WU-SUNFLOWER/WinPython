#ifndef PyList_Hpp
#define PyList_Hpp

#include "PyObject.hpp"

class PyList : public PyObject {
    friend class ListKlass;
private:
    ArrayList<PyObject*>* _container;
public:
    PyList();
    PyList(size_t n);
    PyList(ArrayList<PyObject*>* rawList);
    
    ArrayList<PyObject*>* getContainer() const {
        return _container;
    }
    size_t getLength() const {
        return _container->getLength();
    }
    void append(PyObject* obj) {
        _container->push(obj);
    }
    PyObject* pop() {
        return _container->pop();
    }
    void insert(size_t index, PyObject* elem) {
        _container->insert(index, elem);
    }
    size_t index(PyObject* target);
    PyObject* get(size_t idx) {
        return _container->get(idx);
    }
    void set(size_t idx, PyObject* obj) {
        return _container->set(idx, obj);
    }
};

#endif