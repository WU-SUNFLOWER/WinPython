#ifndef Hpp_PyDict
#define Hpp_PyDict

#include "PyObject.hpp"
#include "map.hpp"

class PyDict : public PyObject {
    friend class DictKlass;
private:
    PyObjectMap* _map = nullptr;
    PyDict() {};
public:

    static PyDict* createDict();
    
    void set(PyObject* key, PyObject* value) {
        _map->set(key, value);
    }
    PyObject* get(PyObject* key) const {
        return _map->get(key);
    }
    PyObject* getKeyByIndex(size_t i) const {
        return _map->getKeyByIndex(i);
    }
    PyObject* getValueByIndex(size_t i) const {
        return _map->getValueByIndex(i);
    }
    bool has(PyObject* key) {
        return _map->has(key);
    }
    PyObject* remove(PyObject* key) {
        return _map->remove(key);
    }

    size_t getSize() const {
        return _map->getLength();
    }
};

class DictIterator :public PyObject {
private:
    PyDict* _owner;
    int _count;
public:
    // DictIterator在初始化时不急着设置klass
    DictIterator(PyDict* owner) : _owner(owner), _count(0) {};
    PyDict* getOwner() {
        return _owner;
    }
    PyObject** getOwnerAddr() {
        return reinterpret_cast<PyObject**>(&_owner);
    }
    int getCount() {
        return _count;
    }
    void increaseCount() {
        ++_count;
    }
};

#endif