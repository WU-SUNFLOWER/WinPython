#include "DictKlass.hpp"
#include "PyTypeObject.hpp"
#include "PyDict.hpp"
#include "PyString.hpp"
#include "nativeFunctions.hpp"
#include "Universe.hpp"
#include <cstdio>
#include <cstdlib>
#include "StringTable.hpp"
#include "ObjectKlass.hpp"
#include "PyInteger.hpp"

DictKlass* DictKlass::instance = nullptr;

DictIteratorKlass<DictIterType::Iter_Keys>* DictIteratorKlass<DictIterType::Iter_Keys>::instance = nullptr;

DictIteratorKlass<DictIterType::Iter_Values>* DictIteratorKlass<DictIterType::Iter_Values>::instance = nullptr;

DictIteratorKlass<DictIterType::Iter_Items>* DictIteratorKlass<DictIterType::Iter_Items>::instance = nullptr;


void DictKlass::initialize() {
    PyDict* dict = new PyDict();

    dict->set(new PyString("setdefault"),
        PackNativeFunc(NativeFunction::dict_set_default));
    dict->set(new PyString("pop"),
        PackNativeFunc(NativeFunction::dict_pop));
    dict->set(new PyString("keys"),
        PackNativeFunc(NativeFunction::dict_keys));
    dict->set(new PyString("values"),
        PackNativeFunc(NativeFunction::dict_values));
    dict->set(new PyString("items"),
        PackNativeFunc(NativeFunction::dict_items));

    setKlassDict(dict);

    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_dict);
    setSuperKlass(ObjectKlass::getInstance());
}

void DictKlass::print(const PyObject* object, int flags) const {
    const PyDict* dict = reinterpret_cast<const PyDict*>(object);
    size_t length = dict->getSize();
    putchar('{');
    for (size_t i = 0; i < length; ++i) {
        PyObject* key = dict->getKeyByIndex(i);
        PyObject* value = dict->getValueByIndex(i);
        key->print();
        printf(": ");
        value->print();
        if (i < length - 1) printf(", ");
    }
    putchar('}');
}

PyObject* DictKlass::len(const PyObject* object) const {
    checkLegalPyObject(object, this);
    return new PyInteger(static_cast<const PyDict*>(object)->getSize());
}

PyObject* DictKlass::subscr(PyObject* object, PyObject* subscription) const {
    checkLegalPyObject(object, this);
    PyObject* ret = static_cast<PyDict*>(object)->get(subscription);
    if (ret != nullptr) return ret;
    printf("Can't find key in your dict: ");
    subscription->print();
    exit(-1);
}

void DictKlass::store_subscr(PyObject* object, PyObject* subscription, PyObject* newObject) const {
    checkLegalPyObject(object, this);
    static_cast<PyDict*>(object)->set(subscription, newObject);
}

void DictKlass::delete_subscr(PyObject* object, PyObject* subscription) const {
    checkLegalPyObject(object, this);
    PyDict* dict = static_cast<PyDict*>(object);
    if (dict->remove(subscription) == nullptr) {
        printf("Can't find key in your dict: ");
        subscription->print();
        exit(-1);
    }
}

template<DictIterType type>
PyObject* DictIteratorKlass<type>::next(PyObject* object) const {
    checkLegalPyObject(object, this);
    
    DictIterator* iter = static_cast<DictIterator*>(object);
    PyDict* dict = iter->getOwner();
    int count = iter->getCount();

    if (count < dict->getSize()) {
        PyObject* ret;
        switch (type) {
            case DictIterType::Iter_Keys:
                ret = dict->getKeyByIndex(count);
                break;
            case DictIterType::Iter_Values:
                ret = dict->getValueByIndex(count);
                break;
            case DictIterType::Iter_Items: {
                PyList* wrapper = new PyList(2);
                wrapper->append(dict->getKeyByIndex(count));
                wrapper->append(dict->getValueByIndex(count));
                ret = wrapper;
                break;
            }
                
        }
        iter->increaseCount();
        return ret;
    }
    else {
        return nullptr;
    }
}

template class DictIteratorKlass<DictIterType::Iter_Keys>;
template class DictIteratorKlass<DictIterType::Iter_Values>;
template class DictIteratorKlass<DictIterType::Iter_Items>;