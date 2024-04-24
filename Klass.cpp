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
#include "TypeKlass.hpp"
#include "Universe.hpp"
#include "StringTable.hpp"
#include "interpreter.hpp"

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

    // ����Python�����Ӧ��type���󣬲�ʵ��type�����C++ Klass��˫���
    PyTypeObject* cls = new PyTypeObject();
    cls->setOwnKlass(newKlass);

    return cls;
}

void Klass::print(const PyObject* lhs, int flags) const {
    Klass* klass = lhs->getKlass();
    putchar('<');

    const PyDict* attrDict = klass->getKlassDict();
    if (attrDict != nullptr) {
        const PyObject* moduleName = attrDict->get(StringTable::str_mod);
        if (moduleName != Universe::PyNone) {
            moduleName->print(FLAG_PyString_PRINT_RAW);
            putchar('.');
        }
    }

    klass->getName()->print(FLAG_PyString_PRINT_RAW);
    printf(" object at 0x%p>", lhs);
}

void Klass::checkLegalPyObject(const PyObject* obj, const Klass* klass) {
    if (obj == nullptr || obj->getKlass() != klass) {
        printf("call IntegerKlass with illegal Python object.");
        exit(-1);
    }
}

PyObject* Klass::allocateInstance(PyObject* callable, PyList* args) {
    checkLegalPyObject(callable, TypeKlass::getInstance());
    // Ϊʵ������Python��������ڴ�
    PyObject* inst = new PyObject();

    // ΪPython�����klass
    PyTypeObject* cls = static_cast<PyTypeObject*>(callable);
    inst->setKlass(cls->getOwnKlass());
    
    // ����Python���__init__������ʼ������
    PyObject* constructor = inst->getattr(StringTable::str_init);
    if (constructor != Universe::PyNone) {
        args->insert(0, inst);
        Interpreter::getInstance()->callVirtual(constructor, args);
    }

    return inst;
}
