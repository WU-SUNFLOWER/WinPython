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

    // GC相关接口
    static void oops_do(OopClosure* closure);
    static void refreshTempRefRecordStack(int, int, int, int);
};

#define NotFound -1

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
        Universe::refreshTempRefRecordStack( \
            _count_temp_objects, _count_temp_pyobject_arrays, \
            _count_temp_klass_arrays, _count_temp_pyobject_map); \
    }

#define isPyInteger(x) ((uintptr_t)(x) & 1)
#define toRawInteger(x) ((int64_t)((int64_t)(x) >> 1))
#define toPyInteger(x) ((PyObject*)(((uintptr_t)(x) << 1) | 1))
#define isPyTrue(x) (isPyInteger(x) ? (toRawInteger(x) != 0) : ((x)->isBoolTrue() == Universe::PyTrue))

#define isCommonFuncKlass(k) \
    (k == FunctionKlass::getInstance() || \
    k == NativeFunctionKlass::getInstance())
#define isNativeFuncKlass(k) (k == NativeFunctionKlass::getInstance())
#define isPythonFuncKlass(k) (k == FunctionKlass::getInstance())
#define isMethod(k) (k == MethodKlass::getInstance())
#define isTypeObject(k) (k == TypeKlass::getInstance())

#endif