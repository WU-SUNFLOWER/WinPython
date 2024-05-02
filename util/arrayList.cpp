#include <cstdio>
#include <cstring>
#include "arrayList.hpp"
#include "PyString.hpp"
#include "Block.hpp"
#include <algorithm>
#include "Klass.hpp"
#include "Universe.hpp"
#include "OopClosure.hpp"

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

template<typename T>
void ArrayList<T>::expand(size_t targetLength) {
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
}

template<typename T>
void ArrayList<T>::push(T elem) {
    if (length >= capacity) {
        expand();
    }
    ptr[length++] = elem;
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
        // 数组满了要先扩容
        if (length >= capacity) expand();
        // 挪动元素
        for (size_t i = length; i > index; --i) {
            ptr[i] = ptr[i - 1];
        }
        ptr[index] = elem;
        ++length;
    }
}

template<typename T>
void ArrayList<T>::set(size_t index, T elem) {
    if (length <= index) {
        expand(index + 1);
        length = index + 1;
    }
    else {
        expand();
    }
    ptr[index] = elem;
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