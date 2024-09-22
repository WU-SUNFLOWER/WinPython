#ifndef RANGEKLASS_HPP
#define RANGEKLASS_HPP

#include "Klass.hpp"
#include "PyRange.hpp"

class RangeKlass : public Klass {
private:
    static RangeKlass* instance;
    RangeKlass();
public:
    static RangeKlass* getInstance() {
        if (instance == nullptr) {
            instance = new RangeKlass();
        }
        return instance;
    }
    virtual void initialize() override;
    virtual void print(const PyObject* object, int flags) const override;
    virtual PyObject* getIter(PyObject* object) const override;
    virtual PyObject* allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args) override;
    
};

class RangeIteratorKlass : public Klass {
private:
    RangeIteratorKlass();
    static RangeIteratorKlass* instance;
public:
    static RangeIteratorKlass* getInstance() {
        if (instance == nullptr) {
            instance = new RangeIteratorKlass();
        }
        return instance;
    }
    virtual PyObject* next(PyObject*) const override;
    virtual PyObject* getIter(PyObject* object) const override {
        return object;
    };
    virtual void oops_do(OopClosure* closure, PyObject* object) override;
};

#endif
