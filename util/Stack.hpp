#ifndef Hpp_Stack

template<typename V>
class Stack {
private:
    V* vector;
    size_t _length;
    size_t _capacity;
public:
    Stack(size_t capacity = 16) : _length(0), _capacity(capacity) {
        vector = new V[capacity];
    }
    ~Stack() {
        delete[] vector;
        _length = _capacity = 0;
    }
    void push(V elem) {
        vector[_length++] = elem;
    }
    V pop() {
        return vector[--_length];
    }
    V top() {
        return vector[_length - 1];
    }
    size_t getLength() {
        return _length;
    }
    size_t getCapacity() {
        return _capacity;
    }
    bool isEmpty() {
        return _length == 0;
    }
};

#endif