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

    _temp_pyobject_array_stack = 
        new Stack<ArrayList<PyObject*>**>(1024 * 1024);
    _temp_klass_array_stack =
        new Stack<ArrayList<Klass*>**>(1024 * 1024);
    _temp_pyobject_map_stack = 
        new Stack<Map<PyObject*, PyObject*>**>(1024 * 1024);
    _temp_stack = 
        new Stack<PyObject**>(2 * 1024 * 1024);
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

    
}

void Universe::destroy() {

}

void Universe::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&PyTrue));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyFalse));
    closure->do_oop(reinterpret_cast<PyObject**>(&PyNone));
    closure->do_array_list(&PyKlasses);

    for (size_t i = 0; i < _temp_stack->getLength(); ++i) {
        closure->do_oop(_temp_stack->get(i));
    }
    for (size_t i = 0; i < _temp_pyobject_array_stack->getLength(); ++i) {
        closure->do_array_list(_temp_pyobject_array_stack->get(i));
    }
    for (size_t i = 0; i < _temp_klass_array_stack->getLength(); ++i) {
        closure->do_array_list(_temp_klass_array_stack->get(i));
    }
    for (size_t i = 0; i < _temp_pyobject_map_stack->getLength(); ++i) {
        closure->do_map(_temp_pyobject_map_stack->get(i));
    }
}

void Universe::refreshTempRefRecordStack(
    int _count_temp_objects, 
    int _count_temp_pyobject_arrays,
    int _count_temp_klass_arrays,
    int _count_temp_pyobject_map
) { 
    _temp_stack->_length -= _count_temp_objects;
    _temp_pyobject_array_stack->_length -= _count_temp_pyobject_arrays;
    _temp_klass_array_stack->_length -= _count_temp_klass_arrays;
    _temp_pyobject_map_stack->_length -= _count_temp_pyobject_map;
}
