#include "PyList.hpp"
#include "ListKlass.hpp"
#include "Universe.hpp"
#include "Handle.hpp"

PyList* PyList::createList(size_t n, bool isInMeta) {
    Handle<PyList*> list = new(isInMeta)PyList();
    list->isInMeta = isInMeta;
    list->setKlass(ListKlass::getInstance());
    Handle<ArrayList<PyObject*>*> container = 
        ArrayList<PyObject*>::createArrayList(nullptr, n, isInMeta);
    list->_container = container;
    return list;
}

PyList* PyList::copyList(Handle<PyList*> src, bool isInMeta) {
    size_t length = src->getLength();
    Handle<PyList*> dst = PyList::createList(length, isInMeta);
    for (size_t i = 0; i < length; ++i) {
        dst->set(i, src->get(i));
    }
    return dst;
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