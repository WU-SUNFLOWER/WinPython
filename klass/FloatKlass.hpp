#ifndef FLOATKLASS_HPP
#define FLOATKLASS_HPP

#include "Klass.hpp"
class PyFloat;

class FloatKlass : public Klass {
private:
    static FloatKlass* instance;

    FloatKlass() {}

public:
    static FloatKlass* getInstance();

    void print(const PyObject* obj, int flags) const;
};

#endif
