#include "TypeKlass.hpp"
#include "PyObject.hpp"
#include "PyTypeObject.hpp"
#include "PyString.hpp"
#include <cassert>
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"
#include "Universe.hpp"
#include "PyDict.hpp"
#include "StringKlass.hpp"
#include <stdlib.h>
#include "nativeFunctions.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "PyMethod.hpp"

TypeKlass* TypeKlass::instance = nullptr;

TypeKlass* TypeKlass::getInstance() {
    if (instance == nullptr) {
        instance = new TypeKlass();
    }
    return instance;
}

size_t TypeKlass::getSize() {
    return sizeof(PyTypeObject);
}

void TypeKlass::print(const PyObject* object, int flag) const {
    checkLegalPyObject(object, this);
    
    const PyTypeObject* typeObject = reinterpret_cast<const PyTypeObject*>(object);
    printf("<class \"");

    const Klass* ownKlass = typeObject->getOwnKlass();
    const PyDict* attrDict = ownKlass->getKlassDict();
    if (attrDict != nullptr) {
        const PyObject* moduleName = attrDict->get(StringTable::str_mod);
        if (moduleName != nullptr) {
            moduleName->print(FLAG_PyString_PRINT_RAW);
            putchar('.');
        }
    }

    ownKlass->getName()->print(FLAG_PyString_PRINT_RAW);
    printf("\">");
}

void TypeKlass::initialize() {
    PyTypeObject* type = new PyTypeObject();
    type->setOwnKlass(this);
    setName(StringTable::str_type);
    addSuper(ObjectKlass::getInstance());
    orderSupers();

    PyDict* klassDict = PyDict::createDict();
    klassDict->set(StringTable::str_mro, 
        PackNativeFunc(NativeFunction::type_object_mro));
    setKlassDict(klassDict);
}

PyObject* TypeKlass::getattr(PyObject* object, PyObject* attr) {
    checkLegalPyObject(object, this);
    assert(attr->getKlass() == StringKlass::getInstance());
    
    PyTypeObject* cls = static_cast<PyTypeObject*>(object);
    PyObject* result = nullptr;

    /* 先顺着type object作为类所持有的mro继承列表查找，看看能不能找到 */
    Klass* ownKlass = cls->getOwnKlass();
    PyList* mroList = ownKlass->mro();
    size_t mroLength = mroList->getLength();
    for (size_t i = 0; i < mroLength; ++i) {
        PyTypeObject* tp = static_cast<PyTypeObject*>(mroList->get(i));
        PyDict* klassDict = tp->getOwnKlass()->getKlassDict();
        if (klassDict != nullptr && (result = klassDict->get(attr))) 
            return result;
    }

    /* 若不能，则把type object当作普通的python对象再进行一次查找 */
    result = find_in_parents(object, attr);
    if (result) {
        if (!isPyInteger(result) && isCommonFuncKlass(result->getKlass())) {
            result = new PyMethod((PyFunction*)result, object);
        }
        return result;
    }

    if (!result) {
        printf("type object '%s' has no attribute '%s'",
            ownKlass->getName()->getValue(),
            static_cast<PyString*>(attr)->getValue());
        exit(-1);
    }
    return result;

}

void TypeKlass::setattr(
    Handle<PyObject*> object, Handle<PyObject*> attr, Handle<PyObject*> value
) {
    checkLegalPyObject(object, this);
    assert(attr->getKlass() == StringKlass::getInstance());

    Handle<PyTypeObject*> cls = static_cast<PyTypeObject*>(object());
    Handle<PyDict*> klassDict = cls->getOwnKlass()->getKlassDict();
    assert(klassDict != nullptr);
    klassDict->set(attr, value);

}

PyObject* TypeKlass::equal(const PyObject* lhs, const PyObject* rhs) const {
    return lhs == rhs ? Universe::PyTrue : Universe::PyFalse;
}

void TypeKlass::oops_do(OopClosure* closure, PyObject* object) {
    // Do nothing, since all klass objects are managed by Universe::Klasses.
}