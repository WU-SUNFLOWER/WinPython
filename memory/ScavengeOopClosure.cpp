#include "ScavengeOopClosure.hpp"
#include "Universe.hpp"
#include "interpreter.hpp"
#include "StringTable.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Block.hpp"
#include "Handle.hpp"

ScavengeOopClosure::ScavengeOopClosure(Space* from, Space* to, Space* meta) :
    _from(from), _to(to), _meta(meta)
{
    _oop_stack = new Stack<PyObject*>(1024 * 1024 * 2);
}

ScavengeOopClosure::~ScavengeOopClosure() {
    _from = _to = _meta = nullptr;
    delete _oop_stack;
    _oop_stack = nullptr;
}

void ScavengeOopClosure::scavenge() {
    process_roots();  
    while (!_oop_stack->isEmpty()) {
        PyObject* object = _oop_stack->pop();
        object->oops_do(static_cast<OopClosure*>(this));
    }
}

void ScavengeOopClosure::process_roots() {
    Universe::oops_do(this);
    Interpreter::getInstance()->oops_do(this);
    HandleMark::getInstance()->oops_do(this);
}

PyObject* ScavengeOopClosure::copy_and_push(PyObject* object) {
    // �����ǰ�����Ѿ������ڵ�survivor�ռ���ȥ�ˣ�
    // ��ֱ�ӻ�ȡ�������µ�ַ����
    void* target = object->getNewAddr();
    if (target) {
        return reinterpret_cast<PyObject*>(target);
    }
    // copy
    size_t size = object->getSize();
    target = _to->allocate(size);
    memcpy(target, object, size);
    // �����µ�ַ����������ٴη���ʱcopy_and_push����ֱ�ӷ����µ�ַ
    object->setNewAddr(target);  
    // push
    _oop_stack->push(reinterpret_cast<PyObject*>(target));

    return reinterpret_cast<PyObject*>(target);
}


void ScavengeOopClosure::do_oop(PyObject** ref) {
    // ����python������ܱ��ദ���ã���֮ǰ�Ѿ����ᵽsurvivor��ȥ��
    // �������ﻹ��Ҫ�ټ�һ��������assert
    if (ref == nullptr || *ref == nullptr  || !Is_Valid_Space_Addr(*ref) || !_from->hasObject(*ref)) {
        return;
    }
    *ref = copy_and_push(*ref);
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass*>** ref) {
    do_array_list_nv<Klass*>(ref);
}

void ScavengeOopClosure::do_array_list(ArrayList<PyObject*>** ref) {
    do_array_list_nv<PyObject*>(ref);
}

/*
void ScavengeOopClosure::do_array_list(ArrayList<PyString*>** ref) {
    do_array_list_nv<PyString*>(ref);
}
*/

template<typename T>
void ScavengeOopClosure::do_array_list_nv(ArrayList<T>** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    assert(_from->hasObject(*ref));

    size_t size = sizeof(ArrayList<T>);
    if (!_to->canAlloc(size)) {
        fputs("Can't allocate more space.", stderr);
        exit(-1);
    }

    void* target = _to->allocate(size);
    memcpy(target, *ref, size);

    *ref = reinterpret_cast<ArrayList<T>*>(target);
    (*ref)->oops_do(this);
}

void ScavengeOopClosure::do_map(Map<PyObject*, PyObject*>** ref) {
    if (ref == nullptr || *ref == nullptr) return;
    assert(_from->hasObject(*ref));

    size_t size = sizeof(Map<PyObject*, PyObject*>);
    if (!_to->canAlloc(size)) {
        fputs("Can't allocate more space.", stderr);
        exit(-1);
    }
    void* target = _to->allocate(size);
    memcpy(target, *ref, size);

    auto old_target = *ref;

    *ref = reinterpret_cast<Map<PyObject*, PyObject*>*>(target);
    (*ref)->oops_do(this);

    if ((*ref)->length > 3) {
        auto x = (*ref)->ptr[3].value->getKlass()->getSize();
    }

}

void ScavengeOopClosure::do_raw_mem(void** ref, size_t length) {
    if (ref == nullptr || *ref == nullptr) return;
    assert(_from->hasObject(*ref));

    if (!_to->canAlloc(length)) {
        fputs("Can't allocate more space.", stderr);
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
