#ifndef PyCell_Hpp
#define PyCell_Hpp

#include "PyObject.hpp"
#include "CellKlass.hpp"
#include "PyList.hpp"

class PyCell: public PyObject{
    friend class CellKlass;
private:
    PyList* _list;
    size_t _index;
public:
    PyCell(PyList* list, size_t index) : _list(list), _index(index) {
        setKlass(CellKlass::getInstance());
    };
    PyList* getList() const {
        return _list;
    }
    PyObject* getObject() const {
        return _list->get(_index);
    }
};
#endif