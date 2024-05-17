#ifndef BOOLKLASS_HPP
#define BOOLKLASS_HPP

#include "Klass.hpp"

class BoolKlass : public Klass {
private:
    static BoolKlass* instance;

    BoolKlass() {}

public:
    static BoolKlass* getInstance();

    void print(const PyObject* obj) const;
};

#endif
