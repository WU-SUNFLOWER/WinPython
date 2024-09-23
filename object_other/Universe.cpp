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
#include "PyBool.hpp"
#include "PyNone.hpp"
#include "BoolKlass.hpp"
#include "NoneKlass.hpp"
#include "FloatKlass.hpp"
#include "ObjectKlass.hpp"
#include "SuperKlass.hpp"
#include "RangeKlass.hpp"

PyObject* Universe::PyTrue = nullptr;
PyObject* Universe::PyFalse = nullptr;
PyObject* Universe::PyNone = nullptr;
Heap* Universe::PyHeap = nullptr;
ArrayList<Klass*>* Universe::PyKlasses = nullptr;
CodeObject* Universe::MainCode = nullptr;

Stack<ArrayList<PyObject*>**>* Universe::_temp_pyobject_array_stack = nullptr;
Stack<ArrayList<Klass*>**>* Universe::_temp_klass_array_stack = nullptr;
Stack<PyObject**>* Universe::_temp_stack = nullptr;
Stack<Map<PyObject*, PyObject*>**>* Universe::_temp_pyobject_map_stack = nullptr;

void Universe::genesis() {
    PyHeap = Heap::getInstance();

    PyKlasses = ArrayList<Klass*>::createArrayList(nullptr);

    StringTable::initialize();

    PyTrue = PyBool::createBool(true);
    PyFalse = PyBool::createBool(false);
    PyNone = PyNone::createNone();

    ObjectKlass::getInstance()->initialize();
    DictKlass::getInstance()->initialize();
    FunctionKlass::getInstance()->initialize();
    IntegerKlass::getInstance()->initialize();
    ListKlass::getInstance()->initialize();
    MethodKlass::getInstance()->initialize();
    NativeFunctionKlass::getInstance()->initialize();
    StringKlass::getInstance()->initialize();
    TypeKlass::getInstance()->initialize();
    BoolKlass::getInstance()->initialize();
    NoneKlass::getInstance()->initialize();
    FloatKlass::getInstance()->initialize();
    SuperKlass::getInstance()->initialize();
    RangeKlass::getInstance()->initialize();
    
}

void Universe::destroy() {

}

void Universe::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&PyTrue));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyFalse));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyNone));
    closure->do_oop(reinterpret_cast<PyObject**>(&MainCode));
    closure->do_array_list(&PyKlasses);
}