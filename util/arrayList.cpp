#include <cstdio>
#include <cstring>
#include "arrayList.hpp"
#include "PyString.hpp"
#include "Block.hpp"
#include <algorithm>

template<typename T>
ArrayList<T>::ArrayList(T defaultElem, int64_t n) {
    capacity = static_cast<size_t>(std::max(1ll, n));
    length = 0;
    _defaultElem = defaultElem;
    ptr = new T[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        ptr[i] = _defaultElem;
    }
}

template<typename T>
ArrayList<T>::~ArrayList() {
    delete[] ptr;
}

template<typename T>
void ArrayList<T>::expand(size_t targetLength) {
    capacity = std::max(capacity << 1, std::max(length + 1, targetLength));
    T* newPtr = new T[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = _defaultElem;
    }
    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = ptr[i];
    }
    delete[] ptr;
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


template class ArrayList<PyString*>;
template class ArrayList<PyObject*>;
template class ArrayList<Block>;