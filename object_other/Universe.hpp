#ifndef Universe_Hpp
#define Universe_Hpp

#include "Stack.hpp"
#include "Heap.hpp"
#include "PyObject.hpp"
#include "arrayList.hpp"
#include "CodeObject.hpp"
#include "OopClosure.hpp"

class Universe {
public:
    static PyObject* PyTrue;
    static PyObject* PyFalse;
    static PyObject* PyNone;
    static Heap* PyHeap;
    static ArrayList<Klass*>* PyKlasses;
    // Interpreter启动时的入口代码，在Interpreter->run的时候设置
    static CodeObject* MainCode;  

    static Stack<ArrayList<PyObject*>**>* _temp_pyobject_array_stack;
    static Stack<ArrayList<Klass*>**>* _temp_klass_array_stack;
    static Stack<PyObject**>* _temp_stack;
    static Stack<Map<PyObject*, PyObject*>**>* _temp_pyobject_map_stack;

    static void genesis();
    static void destroy();

    static void oops_do(OopClosure* closure);
};

#define isTrue(obj) ((obj) == Universe::PyTrue)
#define isFalse(obj) ((obj) == Universe::PyFalse)
#define packBoolean(boolean) ((boolean) ? Universe::PyTrue : Universe::PyFalse)
#define takeNot(obj) ((obj) == Universe::PyTrue ? Universe::PyFalse : Universe::PyTrue)

#define START_COUNT_TEMP_OBJECTS \
    int _count_temp_objects = 0; \
    int _count_temp_pyobject_arrays = 0; \
    int _count_temp_klass_arrays = 0; \
    int _count_temp_pyobject_map = 0;

#define PUSH_TEMP(x) \
    { \
        Universe::_temp_stack->push(reinterpret_cast<PyObject**>(&x)); \
        ++_count_temp_objects; \
    }

#define PUSH_TEMP_PYOBJECT_ARRAY(x) \
    { \
        Universe::_temp_pyobject_array_stack->push(&x); \
        ++_count_temp_pyobject_arrays; \
    }

#define PUSH_TEMP_KLASS_ARRAY(x) \
    { \
        Universe::_temp_klass_array_stack->push(&x); \
        ++_count_temp_klass_arrays; \
    }

#define PUSH_TEMP_PYOBJECT_MAP(x) \
    { \
        Universe::_temp_pyobject_map_stack->push(&x); \
        ++_count_temp_pyobject_map; \
    }

#define END_COUNT_TEMP_OBJECTS \
    { \
        assert(Universe::_temp_stack->_length >= _count_temp_objects); \
        assert(Universe::_temp_pyobject_array_stack->_length >= _count_temp_pyobject_arrays); \
        assert(Universe::_temp_klass_array_stack->_length >= _count_temp_klass_arrays); \
        assert(Universe::_temp_pyobject_map_stack->_length >= _count_temp_pyobject_map); \
        Universe::_temp_stack->_length -= _count_temp_objects; \
        Universe::_temp_pyobject_array_stack->_length -= _count_temp_pyobject_arrays; \
        Universe::_temp_klass_array_stack->_length -= _count_temp_klass_arrays; \
        Universe::_temp_pyobject_map_stack->_length -= _count_temp_pyobject_map; \
    }

#endif