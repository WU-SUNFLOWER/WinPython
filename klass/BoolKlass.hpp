#ifndef BOOLKLASS_HPP
#define BOOLKLASS_HPP

#include "Klass.hpp"

class BoolKlass : public Klass {
private:
    static BoolKlass* instance;

    BoolKlass() {}

public:
    static BoolKlass* getInstance();

    virtual void print(const PyObject* obj,int flags = 0) const override;
    virtual void initialize() override;
    virtual size_t getSize() override;

    PyObject* isBoolTrue(PyObject* object);


    
};



#endif
