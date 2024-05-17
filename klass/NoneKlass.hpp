#ifndef NONEKLASS_HPP
#define NONEKLASS_HPP

#include "Klass.hpp"

class NoneKlass : public Klass {
private:
    static NoneKlass* instance;

    NoneKlass() {}

public:
    static NoneKlass* getInstance();

    virtual void print(const PyObject* obj, int flags = 0) const override;
    virtual void initialize() override;
};

#endif
