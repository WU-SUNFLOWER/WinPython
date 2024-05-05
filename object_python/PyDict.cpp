#include "PyDict.hpp"
#include "DictKlass.hpp"
#include "Universe.hpp"

PyDict* PyDict::createDict() {
    START_COUNT_TEMP_OBJECTS;
    PyDict* dict = new PyDict();
    PUSH_TEMP(dict);
    dict->setKlass(DictKlass::getInstance());
    auto container = Map<PyObject*, PyObject*>::createMap(nullptr);
    dict->_map = container;
    END_COUNT_TEMP_OBJECTS;
    return dict;
}
