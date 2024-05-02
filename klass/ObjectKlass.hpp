#ifndef Hpp_ObjectKlass
#define Hpp_ObjectKlass

#include "Klass.hpp"

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;
public:
    static ObjectKlass* getInstance();
    virtual size_t getSize();
    virtual void oops_do(OopClosure* closure, PyObject* object) override;
};

#endif