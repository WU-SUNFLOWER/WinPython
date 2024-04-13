#include <cstdio>
#include <cstring>
#include "arrayList.hpp"
#include "PyString.hpp"
#include "Block.hpp"
#include <algorithm>

template<typename T>
ArrayList<T>::ArrayList(T defaultElem, size_t n) {
    capacity = n >= 1 ? n : 1;  // ��ֹ�����0�ĳ�ʼ��С
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
void ArrayList<T>::expand() {
    capacity = std::max(capacity << 1, length);
    T* newPtr = new T[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = _defaultElem;
    }
    // �����ϻ�����������Ԫ��
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
    // ��һ�������������β�����Ԫ��
    if (index >= length) {
        set(index, elem);
        return;
    }
    // �ڶ���������������м����Ԫ��
    else {
        // ��������Ҫ������
        if (length >= capacity) expand();
        // Ų��Ԫ��
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
        length = index + 1;
    }
    while (length > capacity) {
        expand();
    }
    ptr[index] = elem;
}

template<typename T>
T ArrayList<T>::get(size_t index) {
    assert(index < length);
    return ptr[index];
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