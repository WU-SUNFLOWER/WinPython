#include "PyNone.hpp"
#include "Universe.hpp"
#include "NoneKlass.hpp"

PyNone* PyNone::createNone() {
    return new PyNone();
}

PyNone::PyNone() {
    setKlass(NoneKlass::getInstance());
}
