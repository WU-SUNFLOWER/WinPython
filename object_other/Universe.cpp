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
CodeObject* Universe::MainCode = nullptr;

Stack<ArrayList<PyObject*>**>* Universe::_temp_array_stack = nullptr;

void Universe::genesis() {
    PyHeap = Heap::getInstance();
    PyKlasses = new ArrayList<Klass*>(nullptr);

    StringTable::initialize();

    _temp_array_stack = new Stack<ArrayList<PyObject*>**>(2 * 1024 * 1024);

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

void Universe::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&PyTrue));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyFalse));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyNone));
    closure->do_oop(reinterpret_cast<PyObject**>(&MainCode));
    closure->do_array_list(&PyKlasses);

    for (size_t i = 0; i < _temp_array_stack->getLength(); ++i) {
        closure->do_array_list(_temp_array_stack->get(i));
    }
}
