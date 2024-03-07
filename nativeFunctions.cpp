#include "nativeFunctions.hpp"
#include "StringKlass.hpp" 
#include "PyString.hpp"

PyObject* NativeFunction::len(PyList* args) {
    return args->get(0)->len();
}

PyObject* NativeFunction::string_upper(PyList* args) {
    PyObject* object = args->get(0);
    assert(object != nullptr && object->getKlass() == StringKlass::getInstance());
    
    PyString& str = *static_cast<PyString*>(object);
    size_t length = str.getLength();
    if (length <= 0) return &str;

    uint8_t* tempBuffer = new uint8_t[length];
    for (size_t i = 0; i < length; ++i) {
        uint8_t ch = str[i];
        tempBuffer[i] = 'a' <= ch && ch <= 'z' ? (ch - 'a' + 'A') : ch;
    }

    PyString* ret = new PyString(tempBuffer, length);
    delete[] tempBuffer;
    return ret;
}