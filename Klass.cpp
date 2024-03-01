#include "Klass.hpp"
#include "PyObject.hpp"
#include <cstdio>
#include <cstdlib>

void Klass::checkLegalPyObject(const PyObject* obj, const Klass* klass)
{
    if (obj == nullptr || obj->getKlass() != klass) {
        printf("call IntegerKlass with illegal Python object.");
        exit(-1);
    }
}