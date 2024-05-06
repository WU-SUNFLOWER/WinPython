#ifndef Hpp_Stack
#define Hpp_Stack

#include <cstdio>
#include <cstdlib>
#include <cassert>

template<typename V>
class Stack {
public:

    V* vector;
    size_t _capacity;
    size_t _length;
    
    Stack(size_t capacity = 16) : _length(0), _capacity(capacity) {
        vector = new V[capacity];
    }
    ~Stack() {
        delete[] vector;
        _length = _capacity = 0;
    }
    void push(V elem) {
        if (_length >= _capacity) {
            printf("GC stack is overflow, whose capacity is %lld\n", _capacity);
            exit(-1);
        }
        vector[_length++] = elem;
    }
    inline V pop() {
        return vector[--_length];
    }
    inline V top() {
        return vector[_length - 1];
    }
    inline V get(size_t idx) {
        assert(0 <= idx && idx < _length);
        return vector[idx];
    }
    inline size_t getLength() {
        return _length;
    }
    inline size_t getCapacity() {
        return _capacity;
    }
    inline bool isEmpty() {
        return _length == 0;
    }
};



#endif