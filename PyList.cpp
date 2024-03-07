#include "PyList.hpp"
#include "ListKlass.hpp"
#include "Universe.hpp"

PyList::PyList(size_t n) {
    _container = new ArrayList<PyObject*>(nullptr, n);
    setKlass(ListKlass::getInstance());
}

PyList::PyList() : PyList(1) {
}

PyList::PyList(ArrayList<PyObject*>* rawList) {
    _container = rawList;
    setKlass(ListKlass::getInstance());
}

size_t PyList::index(PyObject* target) {
    for (size_t i = 0; i < getLength(); ++i) {
        if (target->equal(get(i)) == Universe::PyTrue) {
            return i;
        }
    }
    return -1;
}
