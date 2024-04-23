#include "Klass.hpp"
#include "PyObject.hpp"
#include "DictKlass.hpp"
#include "ListKlass.hpp"
#include "StringKlass.hpp"
#include "PyDict.hpp"
#include "PyList.hpp"
#include "PyString.hpp"
#include <cstdio>
#include <cstdlib>
#include "PyTypeObject.hpp"
#include "ObjectKlass.hpp"

PyObject* Klass::createKlass(PyObject* dict, PyObject* supers, PyObject* name) {
    checkLegalPyObject(dict, DictKlass::getInstance());
    checkLegalPyObject(supers, ListKlass::getInstance());
    checkLegalPyObject(name, StringKlass::getInstance());
    
    PyDict* _dict = static_cast<PyDict*>(dict);
    PyList* _supers = static_cast<PyList*>(supers);
    PyString* _name = static_cast<PyString*>(name);

    Klass* newKlass = new Klass();
    newKlass->setKlassDict(_dict);
    newKlass->setName(_name);
    if (_supers->getLength() > 0) {
        PyTypeObject* super = static_cast<PyTypeObject*>(_supers->get(0));
        newKlass->setSuperKlass(super->getOwnKlass());
    }
    else {
        newKlass->setSuperKlass(ObjectKlass::getInstance());
    }

    // 创建Python世界对应的type对象，并实现type对象和C++ Klass的双向绑定
    PyTypeObject* cls = new PyTypeObject();
    cls->setOwnKlass(newKlass);

    return cls;
}

void Klass::checkLegalPyObject(const PyObject* obj, const Klass* klass) {
    if (obj == nullptr || obj->getKlass() != klass) {
        printf("call IntegerKlass with illegal Python object.");
        exit(-1);
    }
}