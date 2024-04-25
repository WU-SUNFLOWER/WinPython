#include "Universe.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"

PyObject* Universe::PyTrue = nullptr;
PyObject* Universe::PyFalse = nullptr;
PyObject* Universe::PyNone = nullptr;

void Universe::genesis() {
    Universe::PyTrue = new PyInteger(1);
    Universe::PyFalse = new PyInteger(0);
    Universe::PyNone = new PyString("None");
}

void Universe::destroy() {

}