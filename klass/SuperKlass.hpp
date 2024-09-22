#ifndef Hpp_SuperKlass

#include "Klass.hpp"

class SuperKlass : public Klass {
private:
    SuperKlass() {};
    static SuperKlass* instance;
public:
    static SuperKlass* getInstance() {
        if (instance == nullptr) {
            instance = new SuperKlass();
        }
        return instance;
    }
    virtual void initialize() override;
    virtual size_t getSize() override;
    virtual PyObject* getattr(PyObject* obj, PyObject* attr) override;
    virtual PyObject* allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args) override;
};

#endif