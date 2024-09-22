#include "PyDict.hpp"
#include "DictKlass.hpp"
#include "Universe.hpp"
#include "Handle.hpp"

PyDict* PyDict::createDict() {
    Handle<PyDict*> dict = new PyDict();
    dict->setKlass(DictKlass::getInstance());

    auto container = Map<PyObject*, PyObject*>::createMap(nullptr);
    dict->_map = container;

    return dict;
}
