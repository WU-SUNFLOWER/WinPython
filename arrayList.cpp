#include <iostream>
#include "arrayList.hpp"
#include "PyString.hpp"

template<typename T>
ArrayList<T>::ArrayList(size_t n) {
    capacity = n;
    length = 0;
    ptr = new T[capacity];
}

template<typename T>
ArrayList<T>::~ArrayList() {
    delete[] ptr;
}

template<typename T>
void ArrayList<T>::expand() {
    T* newPtr = new T[capacity <<= 1];
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
        // ��װΪ���������һ����Ԫ�أ�������������
        push(nullptr);
        // Ų��Ԫ��
        for (size_t i = length; i > index; --i) {
            ptr[i] = ptr[i - 1];
        }
        ptr[index] = elem;
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

template class ArrayList<PyString*>;
template class ArrayList<PyObject*>;