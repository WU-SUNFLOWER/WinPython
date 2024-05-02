#include "MethodKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyMethod.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

MethodKlass* MethodKlass::instance = nullptr;

size_t MethodKlass::getSize() {
    return sizeof(PyMethod);
}

void MethodKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyMethod* _lhs = static_cast<const PyMethod*>(lhs);
    printf("<method: ");
    _lhs->getFunc()->getName()->print();
    printf(">");
}

void MethodKlass::initialize() {

    (new PyTypeObject())->setOwnKlass(this);

    setName(StringTable::str_method);

    setSuperKlass(ObjectKlass::getInstance());
}

void MethodKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    PyMethod* method = static_cast<PyMethod*>(object);
    closure->do_oop(&method->_owner);
    closure->do_oop(reinterpret_cast<PyObject**>(&method->_func));
}
