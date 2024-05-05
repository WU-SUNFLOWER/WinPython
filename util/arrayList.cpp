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
ArrayList<T>* ArrayList<T>::createArrayList(T defaultElem, int64_t n) {
    puts("Try to create ArrayList with unknown type");
    exit(-1);
}

template<>
ArrayList<PyObject*>* ArrayList<PyObject*>::createArrayList(
    PyObject* defaultElem, int64_t n
) {
    START_COUNT_TEMP_OBJECTS;
    if (defaultElem != nullptr) PUSH_TEMP(defaultElem);
    ArrayList<PyObject*>* object = new ArrayList<PyObject*>();
    PUSH_TEMP_PYOBJECT_ARRAY(object);
    
    object->length = 0;
    object->_defaultElem = defaultElem;
    
    auto capacity = object->capacity = static_cast<size_t>(std::max(1ll, n));
    auto ptr = reinterpret_cast<PyObject**>(
        Universe::PyHeap->allocate(sizeof(PyObject*) * capacity)
    );
    object->ptr = ptr;

    for (size_t i = 0; i < capacity; ++i) {
        ptr[i] = defaultElem;
    }
    
    END_COUNT_TEMP_OBJECTS;
    return object;
}

template<>
ArrayList<Klass*>* ArrayList<Klass*>::createArrayList(
    Klass* defaultElem, int64_t n
) {
    START_COUNT_TEMP_OBJECTS;
    if (defaultElem != nullptr) PUSH_TEMP(defaultElem);
    ArrayList<Klass*>* object = new ArrayList<Klass*>();
    PUSH_TEMP_KLASS_ARRAY(object);

    object->length = 0;
    object->_defaultElem = defaultElem;

    auto capacity = object->capacity = static_cast<size_t>(std::max(1ll, n));
    auto ptr = reinterpret_cast<Klass**>(
        Universe::PyHeap->allocate(sizeof(Klass*) * capacity)
        );
    object->ptr = ptr;

    for (size_t i = 0; i < capacity; ++i) {
        ptr[i] = defaultElem;
    }

    END_COUNT_TEMP_OBJECTS;
    return object;
}

template<>
ArrayList<PyObject*>* ArrayList<PyObject*>::expand(size_t targetLength) {
    START_COUNT_TEMP_OBJECTS;
    
    capacity = std::max(capacity << 1, std::max(length + 1, targetLength));
    ArrayList<PyObject*>* self = this;
    PUSH_TEMP_PYOBJECT_ARRAY(self);

    PyObject** newPtr = reinterpret_cast<PyObject**>(
        Universe::PyHeap->allocate(sizeof(PyObject*) * capacity));
    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = self->_defaultElem;
    }
    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = self->ptr[i];
    }
    self->ptr = newPtr;

    END_COUNT_TEMP_OBJECTS;
    return self;
}

template<>
ArrayList<Klass*>* ArrayList<Klass*>::expand(size_t targetLength) {
    START_COUNT_TEMP_OBJECTS;

    capacity = std::max(capacity << 1, std::max(length + 1, targetLength));
    ArrayList<Klass*>* self = this;
    PUSH_TEMP_KLASS_ARRAY(self);

    Klass** newPtr = reinterpret_cast<Klass**>(
        Universe::PyHeap->allocate(sizeof(Klass*) * capacity));
    for (size_t i = 0; i < capacity; ++i) {
        newPtr[i] = self->_defaultElem;
    }
    // 拷贝老缓冲区的数组元素
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = self->ptr[i];
    }
    self->ptr = newPtr;

    END_COUNT_TEMP_OBJECTS;
    return self;
}

template<>
void ArrayList<PyObject*>::push(PyObject* elem) {
    auto self = this;
    if (length >= capacity) {
        START_COUNT_TEMP_OBJECTS;
        PUSH_TEMP(self);
        PUSH_TEMP(elem);
        expand();
        END_COUNT_TEMP_OBJECTS;
    }
    self->ptr[self->length++] = elem;
}

template<typename T>
void ArrayList<T>::push(T elem) {
    auto self = this;
    if (length >= capacity) {
        self = expand();
    }
    self->ptr[self->length++] = elem;
}



template<>
void ArrayList<PyObject*>::insert(size_t index, PyObject* elem) {
    // 第一种情况：在数组尾端添加元素
    if (index >= length) {
        set(index, elem);
        return;
    }
    // 第二种情况：在数组中间添加元素
    else {
        auto self = this;
        size_t length = self->length;
        // 数组满了要先扩容
        if (length >= self->capacity) {
            START_COUNT_TEMP_OBJECTS;
            PUSH_TEMP(self);
            PUSH_TEMP(elem);
            self->expand();
            END_COUNT_TEMP_OBJECTS;
        }
        // 挪动元素
        for (size_t i = length; i > index; --i) {
            self->ptr[i] = self->ptr[i - 1];
        }
        self->ptr[index] = elem;
        ++(self->length);
    }
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
        size_t length = self->length;
        // 数组满了要先扩容
        if (length >= capacity) {
            self = self->expand();
        }
        // 挪动元素
        for (size_t i = length; i > index; --i) {
            self->ptr[i] = self->ptr[i - 1];
        }
        self->ptr[index] = elem;
        ++(self->length);
    }
}


template<>
void ArrayList<PyObject*>::set(size_t index, PyObject* elem) {
    auto self = this;
    if (self->capacity <= index) {
        START_COUNT_TEMP_OBJECTS;
        PUSH_TEMP(elem);
        PUSH_TEMP(self);
        expand(index + 1);
        END_COUNT_TEMP_OBJECTS;
    }
    self->length = std::max(self->length, index + 1);
    self->ptr[index] = elem;
}

template<typename T>
void ArrayList<T>::set(size_t index, T elem) {
    auto self = this;
    if (length <= index) {
        self = self->expand(index + 1);
        self->length = index + 1;
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
template class ArrayList<PyObject*>;