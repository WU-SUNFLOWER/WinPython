#include "PyList.hpp"
#include "ListKlass.hpp"
#include "Universe.hpp"
#include "Handle.hpp"

PyList* PyList::createList(size_t n, bool isInMeta) {
    //START_COUNT_TEMP_OBJECTS;
    Handle<PyList*> list = new(isInMeta)PyList();
    list->isInMeta = isInMeta;
    list->setKlass(ListKlass::getInstance());
    //PUSH_TEMP(list);
    auto container = ArrayList<PyObject*>::createArrayList(nullptr, n, isInMeta);
    list->_container = container;
    //END_COUNT_TEMP_OBJECTS;
    return list;
}

PyList* PyList::copyList(PyList* src, bool isInMeta) {
    START_COUNT_TEMP_OBJECTS;
    PUSH_TEMP(src);
    size_t length = src->getLength();
    PyList* dst = PyList::createList(length, isInMeta);
    for (size_t i = 0; i < length; ++i) {
        dst->set(i, src->get(i));
    }
    END_COUNT_TEMP_OBJECTS;
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