#ifndef Hpp_ScavengeOopClosure

#include "OopClosure.hpp"
#include "Space.hpp"
#include "Stack.hpp"

class ScavengeOopClosure : public OopClosure {
public:
    Space* _from;
    Space* _to;
    Space* _meta;

    Stack<PyObject*>* _oop_stack;

    PyObject* copy_and_push(PyObject* object);

public:
    ScavengeOopClosure(Space* from, Space* to, Space* meta);
    virtual ~ScavengeOopClosure();

    virtual void do_oop(PyObject** addr) override;

    template <typename T>
    void do_array_list_nv(ArrayList<T>** alist);
    virtual void do_array_list(ArrayList<Klass*>**) override;
    virtual void do_array_list(ArrayList<PyObject*>**) override;
    //virtual void do_array_list(ArrayList<PyString*>**) override;

    virtual void do_map(Map<PyObject*, PyObject*>** addr) override;
    virtual void do_raw_mem(void** addr, size_t length) override;
    virtual void do_klass(Klass** addr) override;

    void scavenge();
    void process_roots();
};

#endif