#include "ScavengeOopClosure.hpp"
#include "Universe.hpp"
#include "interpreter.hpp"
#include "StringTable.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>

ScavengeOopClosure::ScavengeOopClosure(Space* from, Space* to, Space* meta) :
    _from(from), _to(to), _meta(meta)
{
    _oop_stack = new Stack<PyObject*>(1024);
}

ScavengeOopClosure::~ScavengeOopClosure() {
    _from = _to = _meta = nullptr;
    delete _oop_stack;
    _oop_stack = nullptr;
}

void ScavengeOopClosure::scavenge() {
    process_roots();  
    while (!_oop_stack->isEmpty()) {
        _oop_stack->pop()->oops_do(static_cast<OopClosure*>(this));
    }
}

void ScavengeOopClosure::process_roots() {
    Universe::oops_do(this);
    Interpreter::getInstance()->oops_do(this);
    StringTable::oops_do(this);
}

PyObject* ScavengeOopClosure::copy_and_push(PyObject* object) {
    // 如果当前对象已经被倒腾到survivor空间中去了，
    // 则直接获取并返回新地址即可
    void* target = object->getNewAddr();
    if (target) {
        return reinterpret_cast<PyObject*>(target);
    }
    // copy
    size_t size = object->getSize();
    target = _to->allocate(size);
    memcpy(target, object, size);
    // 设置新地址，方便后续再次访问时copy_and_push函数直接返回新地址
    object->setNewAddr(target);  
    // push
    _oop_stack->push(reinterpret_cast<PyObject*>(target));

    return reinterpret_cast<PyObject*>(target);
}


void ScavengeOopClosure::do_oop(PyObject** reference) {
    if (reference == nullptr || *reference == nullptr) {
        return;
    }
    if (!_from->hasObject(*reference)) {
        return;
    }
    *reference = copy_and_push(*reference);
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass*>** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    //if (!_from->hasObject(*ref)) return;
    assert(_from->hasObject(*ref));
    
    size_t size = sizeof(ArrayList<Klass*>);
    if (!_to->canAlloc(size)) {
        puts("Can't allocate more space.");
        exit(-1);
    }
    void* target = _to->allocate(size);
    memcpy(target, *ref, size);
    *ref = reinterpret_cast<ArrayList<Klass*>*>(target);
    (*ref)->oops_do(this);
}

void ScavengeOopClosure::do_array_list(ArrayList<PyObject*>** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    if (!_from->hasObject(*ref)) return;
    size_t size = sizeof(ArrayList<PyObject*>);
    if (!_to->canAlloc(size)) {
        puts("Can't allocate more space.");
        exit(-1);
    }
    void* target = _to->allocate(size);
    memcpy(target, *ref, size);
    *ref = reinterpret_cast<ArrayList<PyObject*>*>(target);
    (*ref)->oops_do(this);
}

void ScavengeOopClosure::do_map(Map<PyObject*, PyObject*>** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    if (!_from->hasObject(*ref)) return;
    size_t size = sizeof(Map<PyObject*, PyObject*>);
    if (!_to->canAlloc(size)) {
        puts("Can't allocate more space.");
        exit(-1);
    }
    void* target = _to->allocate(size);
    memcpy(target, *ref, size);
    *reinterpret_cast<void**>(ref) = target;
    (*ref)->oops_do(this);
}

void ScavengeOopClosure::do_raw_mem(void** ref, size_t length) {
    if (ref == nullptr || *ref == nullptr) return;
    if (!_from->hasObject(*ref)) return;
    if (!_to->canAlloc(length)) {
        puts("Can't allocate more space.");
        exit(-1);
    }
    void* target = _to->allocate(length);
    memcpy(target, *ref, length);
    *ref = target;
}

void ScavengeOopClosure::do_klass(Klass** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    (*ref)->oops_do(this);
}
