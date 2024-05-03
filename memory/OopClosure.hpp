#ifndef Hpp_OopClosure
#define Hpp_OopClosure

#include "arrayList.hpp"
#include "map.hpp"
#include "Block.hpp"

class PyObject;
class Klass;

class OopClosure {
public:
    virtual void do_oop(PyObject** object) = 0;

    virtual void do_array_list(ArrayList<Klass*>** list) = 0;
    virtual void do_array_list(ArrayList<PyObject*>** list) = 0;
    virtual void do_array_list(ArrayList<Block>**) = 0;
    
    virtual void do_map(Map<PyObject*, PyObject*>** map) = 0;
    virtual void do_raw_mem(void** mem, size_t size) = 0;
    virtual void do_klass(Klass** klass) = 0;
};

#endif