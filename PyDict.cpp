#include "PyDict.hpp"
#include "DictKlass.hpp"

PyDict::PyDict() {
    _map = new PyObjectMap();
    setKlass(DictKlass::getInstance());
}
