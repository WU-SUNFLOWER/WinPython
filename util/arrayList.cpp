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

template<typename T>
ArrayList<T>::ArrayList(T defaultElem, int64_t n) {
    capacity = static_cast<size_t>(std::max(1ll, n));
    length = 0;
    _defaultElem = defaultElem;
    void* addr = Universe::PyHeap->allocate(sizeof(T) * capacity);
    ptr = new(addr)T[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        ptr[i] = _defaultElem;
    }
}

template<typename T>
ArrayList<T>::~ArrayList() {

}

template<>
ArrayList<PyObject*>* ArrayList<PyObject*>::expand(size_t targetLength) {
    capacity = std::max(capacity << 1, std::max(length + 1, targetLength));

    ArrayList<PyObject*>* self = this;

    Universe::_temp_array_stack->push(&self);

    PyObject** newPtr = reinterpret_cast<PyObject**>(
        Universe::PyHeap->allocate(sizeof(PyObject*) * capacity));

    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = _defaultElem;
    }
    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = self->ptr[i];
    }
    self->ptr = newPtr;

    Universe::_temp_array_stack->pop();

    return self;
}

template<typename T>
ArrayList<T>* ArrayList<T>::expand(size_t targetLength) {
    capacity = std::max(capacity << 1, std::max(length + 1, targetLength));
    
    void* addr = Universe::PyHeap->allocate(sizeof(T) * capacity);
    T* newPtr = new(addr)T[capacity];

    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = _defaultElem;
    }
    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = ptr[i];
    }
    ptr = newPtr;
    return this;
}

template<typename T>
void ArrayList<T>::push(T elem) {
    auto self = this;
    if (length >= capacity) {
        self = expand();
    }
    self->ptr[length++] = elem;
}

template<typename T>
void ArrayList<T>::insert(size_t index, T elem) {
    // 第一种情况：在数组尾端添加元素
    if (index >= length) {
        set(index, elem);
        return;
    }
    // 第二种情况：在数组中间添加元素
    else {
        auto self = this;
        // 数组满了要先扩容
        if (length >= capacity) self->expand();
        // 挪动元素
        for (size_t i = length; i > index; --i) {
            self->ptr[i] = self->ptr[i - 1];
        }
        self->ptr[index] = elem;
        ++length;
    }
}

template<>
void ArrayList<PyObject*>::set(size_t index, PyObject* elem) {
    PUSH_TEMP(elem);
    auto self = this;
    if (self->length <= index) {
        self = expand(index + 1);
        self->length = index + 1;
    }
    else {
        self = expand();
    }
    self->ptr[index] = elem;
    POP_TEMP(1);
}

template<typename T>
void ArrayList<T>::set(size_t index, T elem) {
    auto self = this;
    if (length <= index) {
        self->expand(index + 1);
        length = index + 1;
    }
    self->ptr[index] = elem;
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
    closure->do_raw_mem(reinterpret_cast<void**>(&ptr),
        capacity * sizeof(T));
}

template<typename T>
void* ArrayList<T>::operator new(size_t size) {
    return Universe::PyHeap->allocate(size);
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
    if (!addr) return;
    _mark_word = reinterpret_cast<uintptr_t>(addr) | 0x2;
}

template<>
void ArrayList<Klass*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem(reinterpret_cast<void**>(&ptr), 
        capacity * sizeof(Klass*));
    for (size_t i = 0; i < length; ++i) {
        closure->do_klass(&ptr[i]);
    }
}

template<>
void ArrayList<PyObject*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem(reinterpret_cast<void**>(&ptr),
        capacity * sizeof(PyObject*));
    for (size_t i = 0; i < length; ++i) {
        closure->do_oop(&ptr[i]);
    }
}

template class ArrayList<Klass*>;
template class ArrayList<PyString*>;
template class ArrayList<PyObject*>;
template class ArrayList<Block>;
template class ArrayList<PyObject**>;