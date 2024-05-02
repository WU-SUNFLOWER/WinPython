#include "CellKlass.hpp"
#include "PyCell.hpp"

CellKlass* CellKlass::instance = nullptr;

size_t CellKlass::getSize() {
    return sizeof(PyCell);
}

void CellKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    PyCell* cell = static_cast<PyCell*>(object);
    closure->do_oop(reinterpret_cast<PyObject**>(&cell->_list));
}
