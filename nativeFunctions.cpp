#include "nativeFunctions.hpp"
#include "StringKlass.hpp" 
#include "PyString.hpp"
#include "Universe.hpp"
#include "PyInteger.hpp"
#include <algorithm>

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

PyObject* NativeFunction::list_append(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    list->append(args->get(1));
    return Universe::PyNone;
}

PyObject* NativeFunction::list_insert(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    PyInteger* integer = reinterpret_cast<PyInteger*>(args->get(1));
    // Python����������±�Խ�磬��ǿ�Ʋ��뵽���һ��
    size_t pos = std::min(
        static_cast<size_t>(integer->getValue()), 
        list->getLength());
    list->insert(pos, args->get(2));
    return Universe::PyNone;
}

PyObject* NativeFunction::list_index(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    size_t ans = list->index(args->get(1));
    return new PyInteger(ans);
}

PyObject* NativeFunction::list_pop(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    return list->pop();
}

PyObject* NativeFunction::list_remove(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    PyObject* target = args->get(1);
    for (size_t i = 0; i < list->getLength(); ++i) {
        if (list->get(i)->equal(target) == Universe::PyTrue) {
            list->deleteByIndex(i--);
        }
    }
    return Universe::PyNone;
}

PyObject* NativeFunction::list_reverse(PyList* args) {
    PyList* list = reinterpret_cast<PyList*>(args->get(0));
    size_t length = list->getLength();
    for (size_t i = 0; i < length / 2; ++i) {
        PyObject* temp = list->get(i);
        list->set(i, list->get(length - i - 1));
        list->set(length - i - 1, temp);
    }
    return Universe::PyNone;
}