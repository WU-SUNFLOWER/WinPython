#include "Universe.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "StringTable.hpp"
#include "DictKlass.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "ListKlass.hpp"
#include "MethodKlass.hpp"
#include "StringKlass.hpp"
#include "TypeKlass.hpp"

PyObject* Universe::PyTrue = nullptr;
PyObject* Universe::PyFalse = nullptr;
PyObject* Universe::PyNone = nullptr;
Heap* Universe::PyHeap = nullptr;
ArrayList<Klass*>* Universe::PyKlasses = nullptr;

void Universe::genesis() {
    PyHeap = Heap::getInstance();
    PyKlasses = new ArrayList<Klass*>(nullptr);

    StringTable::initialize();

    PyTrue = new PyInteger(1);
    PyFalse = new PyInteger(0);
    PyNone = new PyString("None");

    DictKlass::getInstance()->initialize();
    FunctionKlass::getInstance()->initialize();
    IntegerKlass::getInstance()->initialize();
    ListKlass::getInstance()->initialize();
    MethodKlass::getInstance()->initialize();
    NativeFunctionKlass::getInstance()->initialize();
    StringKlass::getInstance()->initialize();
    TypeKlass::getInstance()->initialize();


    
}

void Universe::destroy() {

}