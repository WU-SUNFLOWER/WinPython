#ifndef PyList_Hpp
#define PyList_Hpp

#include "PyObject.hpp"

class PyList : public PyObject {
    friend class ListKlass;
//private:
public:
    ArrayList<PyObject*>* _container = nullptr;
private:
    PyList() {}
public:
    static PyList* createList(size_t n = 8, bool isInMeta = false);

    static PyList* copyList(Handle<PyList*> src, bool isInMeta = false);
    
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
    PyObject* top() {
        return _container->top();
    }
    void insert(size_t index, PyObject* elem) {
        _container->insert(index, elem);
    }
    size_t index(PyObject* target);
    PyObject* get(size_t idx) const {
        return _container->get(idx);
    }
    void set(size_t idx, PyObject* obj) {
        _container->set(idx, obj);
        return;
    }
    void deleteByIndex(size_t idx) {
        _container->deleteByIndex(idx);
    }
};

class ListIterator : public PyObject {
    friend class ListIteratorKlass;
private:
    PyList* _owner;
    int _count;
public:
    ListIterator(PyList* owner);
    PyList* getOwner() {
        return _owner;
    }
    int getCount() {
        return _count;
    }
    void increaseCount() {
        ++_count;
    }
};

#endif