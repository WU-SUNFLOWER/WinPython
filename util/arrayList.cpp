#include <cstdio>
#include <cstring>
#include "arrayList.hpp"
#include "PyString.hpp"
#include "Block.hpp"
#include <algorithm>
#include "Klass.hpp"
#include "Universe.hpp"
#include "OopClosure.hpp"
#include "interpreter.hpp"
#include "Handle.hpp"

template<typename T>
ArrayList<T>* ArrayList<T>::createArrayList(
    T defaultElem, int64_t n
) {
    Handle<T> _defaultElem = defaultElem;
    Handle<ArrayList<T>*> object = new ArrayList<T>();

    object->length = 0;
    object->_defaultElem = _defaultElem;
    object->capacity = static_cast<size_t>(std::max(1ll, n));

    void* addr = Universe::PyHeap->allocate(sizeof(T) * object->capacity);
    object->ptr = new(addr)T[object->capacity];

    for (size_t i = 0; i < object->capacity; ++i) {
        object->ptr[i] = _defaultElem;
    }

    return object;
}

template <typename T>
void* ArrayList<T>::operator new(size_t size) {
    return Universe::PyHeap->allocate(size);
}

template<typename T>
void ArrayList<T>::expand(size_t targetLength) {
    Handle<ArrayList<T>*> self = this;
    self->capacity = std::max(self->capacity << 1, std::max(self->length + 1, targetLength));

    assert(self->length <= self->capacity);
    void* addr = Universe::PyHeap->allocate(sizeof(T) * self->capacity);
    T* newPtr = new(addr)T[self->capacity];

    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < self->length; ++i) {
        newPtr[i] = self->ptr[i];
    }
    // 设置默认值
    for (size_t i = self->length; i < self->capacity; ++i) {
        newPtr[i] = self->_defaultElem;
    }

    self->ptr = newPtr;
}

template<typename T>
void ArrayList<T>::push(T elem) {
    Handle<ArrayList<T>*> self = this;
    Handle<T> _elem = elem;
    if (self->length >= self->capacity) {
        self->expand();
    }
    self->ptr[self->length++] = _elem;
}

template<typename T>
void ArrayList<T>::insert(size_t index, T elem) {
    Handle<ArrayList<T>*> self = this;
    Handle<T> _elem = elem;
    // 第一种情况：在数组尾端添加元素
    if (index >= self->length) {
        self->set(index, _elem);
        return;
    }
    // 第二种情况：在数组中间添加元素
    else {
        // 数组满了要先扩容
        if (self->length >= self->capacity) {
            self->expand();
        }
        // 挪动元素
        for (size_t i = self->length; i > index; --i) {
            self->ptr[i] = self->ptr[i - 1];
        }
        self->ptr[index] = _elem;
        ++(self->length);
    }
}

template<typename T>
void ArrayList<T>::set(size_t index, T elem) {
    Handle<ArrayList<T>*> self = this;
    Handle<T> _elem = elem;
    if (self->length <= index) {
        self->expand(index + 1);
        self->length = index + 1;
    }
    self->ptr[index] = _elem;
}

template<typename T>
T ArrayList<T>::get(size_t index) {
    return index < length ? ptr[index] : _defaultElem;
}

template<typename T>
void ArrayList<T>::deleteByIndex(size_t idx) {
    for (size_t i = idx; i < length - 1; ++i) {
        ptr[i] = ptr[i + 1];
    }
    --length;
}

template<typename T>
void ArrayList<T>::oops_do(OopClosure* closure) {
    fputs("Can't call ArrayList<T>::oops_do(OopClosure* closure) directly", stderr);
}

template<typename T>
void* ArrayList<T>::getNewAddr() {
    if ((_mark_word & 0x2) == 0x2) {
        return reinterpret_cast<void*>(_mark_word & ~7);
    }
    return nullptr;
}

template<typename T>
void ArrayList<T>::setNewAddr(void* addr) {
    assert(addr != nullptr);
    _mark_word = reinterpret_cast<uintptr_t>(addr) | 0x2;
}

template<typename T>
size_t ArrayList<T>::getSize() {
    return sizeof(ArrayList<T>);
}

template<>
void ArrayList<Klass*>::oops_do(OopClosure* closure) {
    assert(Universe::PyHeap->survivor->hasObject(this));
    if (ptr == nullptr) return;
    closure->do_raw_mem(reinterpret_cast<void**>(&ptr), 
        capacity * sizeof(Klass*));
    for (size_t i = 0; i < length; ++i) {
        closure->do_klass(&(ptr[i]));
    }
}

template<>
void ArrayList<PyObject*>::oops_do(OopClosure* closure) {
    assert(Universe::PyHeap->survivor->hasObject(this));
    if (ptr == nullptr) return;
    closure->do_raw_mem(reinterpret_cast<void**>(&ptr),
        capacity * sizeof(PyObject*));
    for (size_t i = 0; i < length; ++i) {
        closure->do_oop((&ptr[i]));
    }
}

template class ArrayList<Klass*>;
template class ArrayList<PyObject*>;