#include "PyDict.hpp"
#include "DictKlass.hpp"

PyDict::PyDict() {
    _map = new PyObjectMap(nullptr);
    setKlass(DictKlass::getInstance());
}
