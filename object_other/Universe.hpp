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

    static Stack<ArrayList<PyObject*>**>* _temp_array_stack;

    static void genesis();
    static void destroy();

    static void oops_do(OopClosure* closure);
};

#define isTrue(obj) ((obj) == Universe::PyTrue)
#define isFalse(obj) ((obj) == Universe::PyFalse)
#define packBoolean(boolean) ((boolean) ? Universe::PyTrue : Universe::PyFalse)
#define takeNot(obj) ((obj) == Universe::PyTrue ? Universe::PyFalse : Universe::PyTrue)

#endif