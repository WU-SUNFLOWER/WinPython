#ifndef Universe_Hpp
#define Universe_Hpp

#include "Heap.hpp"
#include "PyObject.hpp"
#include "arrayList.hpp"

/*
namespace Universe {
    extern PyObject* PyTrue;
    extern PyObject* PyFalse;
    extern PyObject* PyNone;
    extern Heap* PyHeap;
    extern ArrayList<Klass*>* PyKlasses;
    void genesis();
    void destroy();
}
*/

class Universe {
public:
    static PyObject* PyTrue;
    static PyObject* PyFalse;
    static PyObject* PyNone;
    static Heap* PyHeap;
    static ArrayList<Klass*>* PyKlasses;
    
    static void genesis();
    static void destroy();
};

#define isTrue(obj) ((obj) == Universe::PyTrue)
#define isFalse(obj) ((obj) == Universe::PyFalse)
#define packBoolean(boolean) ((boolean) ? Universe::PyTrue : Universe::PyFalse)
#define takeNot(obj) ((obj) == Universe::PyTrue ? Universe::PyFalse : Universe::PyTrue)

#endif