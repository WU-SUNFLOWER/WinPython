#include "PyList.hpp"
#include "ListKlass.hpp"
#include "Universe.hpp"

PyList* PyList::createList(size_t n) {
    START_COUNT_TEMP_OBJECTS;
    PyList* list = new PyList();
    list->setKlass(ListKlass::getInstance());
    PUSH_TEMP(list);
    auto container = ArrayList<PyObject*>::createArrayList(nullptr, n);
    list->_container = container;
    END_COUNT_TEMP_OBJECTS;
    return list;
}

size_t PyList::index(PyObject* target) {
    for (size_t i = 0; i < getLength(); ++i) {
        if (target->equal(get(i)) == Universe::PyTrue) {
            return i;
        }
    }
    return -1;
}

ListIterator::ListIterator(PyList* owner)
    : _count(0), _owner(owner)
{
    setKlass(ListIteratorKlass::getInstance());
}