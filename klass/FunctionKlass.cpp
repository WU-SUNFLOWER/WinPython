#include "FunctionKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

FunctionKlass* FunctionKlass::instance = nullptr;

size_t FunctionKlass::getSize() {
    return sizeof(PyFunction);
}

void FunctionKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_function);
    setSuperKlass(ObjectKlass::getInstance());
}

void FunctionKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<function: ");
    _lhs->funcName->print();
    printf(">");
}

void FunctionKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    PyFunction* func = static_cast<PyFunction*>(object);
    closure->do_oop(reinterpret_cast<PyObject**>(&func->funcCode));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->funcName));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_globals));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_defaultArgs));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_freevars));

}
