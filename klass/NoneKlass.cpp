#include "NoneKlass.hpp"
#include <stdio.h>
#include "PyTypeObject.hpp"
#include "StringTable.hpp"
#include "PyDict.hpp"
#include "ObjectKlass.hpp"


NoneKlass* NoneKlass::instance = nullptr;

NoneKlass* NoneKlass::getInstance() {
    if (instance == nullptr) {
        instance = new NoneKlass();
    }
    return instance;
}

void NoneKlass::print(const PyObject* obj,int) const {
    printf("None");
}

void NoneKlass::initialize() {
    PyDict* dict = PyDict::createDict();


    setKlassDict(dict);

    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_bool);
    setSuperKlass(ObjectKlass::getInstance());
}