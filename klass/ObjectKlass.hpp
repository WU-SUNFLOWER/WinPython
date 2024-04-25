#ifndef Hpp_ObjectKlass
#define Hpp_ObjectKlass

#include "Klass.hpp"

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;
public:
    static ObjectKlass* getInstance();
};

#endif