#include "nativeFunctions.hpp"
#include "StringKlass.hpp" 
#include "PyString.hpp"
#include "Universe.hpp"
#include "PyInteger.hpp"
#include "PyDict.hpp"
#include "DictKlass.hpp"
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
    // Python中如果插入下标越界，则强制插入到最后一个
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

PyObject* NativeFunction::list_iterator_next(PyList* args) {
    ListIterator* iter = static_cast<ListIterator*>(args->get(0));
    return iter->next();
}

// dict.setdefault(key, value)
// 先看看字典中是否有key字段，如有则啥也不干
// 如果没有，则向字典添加键值对(key, value)
PyObject* NativeFunction::dict_set_default(PyList* args) {
    PyDict* dict = static_cast<PyDict*>(args->get(0));
    PyObject* key = args->get(1);

    if (!dict->has(key)) {
        dict->set(key, args->get(2));
    }

    return Universe::PyNone;
}

PyObject* NativeFunction::dict_pop(PyList* args) {
    PyDict* dict = static_cast<PyDict*>(args->get(0));
    PyObject* key = args->get(1);
    PyObject* result = dict->remove(key);
    if (result == Universe::PyNone) {
        printf("Can't find key in your dict: ");
        key->print();
        exit(-1);
    }
    return result;
}

PyObject* NativeFunction::dict_keys(PyList* args) {
    PyDict* dict = static_cast<PyDict*>(args->get(0));
    DictIterator* iter = new DictIterator(dict);
    iter->setKlass(DictIteratorKlass<DictIterType::Iter_Keys>::getInstance());
    return iter;
}

PyObject* NativeFunction::dict_values(PyList* args) {
    PyDict* dict = static_cast<PyDict*>(args->get(0));
    DictIterator* iter = new DictIterator(dict);
    iter->setKlass(DictIteratorKlass<DictIterType::Iter_Values>::getInstance());
    return iter;
}

PyObject* NativeFunction::dict_items(PyList* args) {
    PyDict* dict = static_cast<PyDict*>(args->get(0));
    DictIterator* iter = new DictIterator(dict);
    iter->setKlass(DictIteratorKlass<DictIterType::Iter_Items>::getInstance());
    return iter;
}