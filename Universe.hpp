#ifndef Universe_Hpp
#define Universe_Hpp

#include "PyObject.hpp"

namespace Universe {
    extern PyObject* PyTrue;
    extern PyObject* PyFalse;
    extern PyObject* PyNone;
    void genesis();
    void destroy();
}

#define isTrue(obj) ((obj) == Universe::PyTrue)
#define isFalse(obj) ((obj) == Universe::PyFalse)
#define packBoolean(boolean) ((boolean) ? Universe::PyTrue : Universe::PyFalse)
#define takeNot(obj) ((obj) == Universe::PyTrue ? Universe::PyFalse : Universe::PyTrue)

#endif