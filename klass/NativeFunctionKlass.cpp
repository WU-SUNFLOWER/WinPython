#include "NativeFunctionKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"

NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

void NativeFunctionKlass::print(const PyObject* lhs, int flags) const {
    checkLegalPyObject(lhs, this);
    const PyFunction* _lhs = static_cast<const PyFunction*>(lhs);
    printf("<native function: ");
    _lhs->funcName->print();
    printf(">");
}

void NativeFunctionKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    PyFunction* func = static_cast<PyFunction*>(object);
    closure->do_oop(reinterpret_cast<PyObject**>(&func->funcCode));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->funcName));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_globals));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_defaultArgs));
    closure->do_oop(reinterpret_cast<PyObject**>(&func->_freevars));
}

size_t NativeFunctionKlass::getSize() {
    return sizeof(PyFunction);
}

void NativeFunctionKlass::initialize() {

    (new PyTypeObject())->setOwnKlass(this);

    setName(StringTable::str_builtin_func);

    setSuperKlass(ObjectKlass::getInstance());
}