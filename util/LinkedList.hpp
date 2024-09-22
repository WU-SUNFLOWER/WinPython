#ifndef LinkedList_Hpp
#define LinkedList_Hpp

#include <cassert>
#include "OopClosure.hpp"

class LinkedList {
private:
    LinkedList* _prev;
    LinkedList* _next;
public:
    LinkedList() {
        _prev = _next = this;
    }
    ~LinkedList() = default;
    void insert_between(LinkedList* prev, LinkedList* next) {
        assert(prev != nullptr);
        assert(next != nullptr);
        prev->_next = this;
        next->_prev = this;
        this->_prev = prev;
        this->_next = next;
    }
    void add_before(LinkedList* other) {
        assert(other != nullptr);
        assert(other->_prev != nullptr);
        insert_between(other->_prev, other);
    }
    void add_after(LinkedList* other) {
        assert(other != nullptr);
        assert(other->_next != nullptr);
        insert_between(other, other->_next);
    }
    void del() {
        assert(_prev != nullptr);
        assert(_next != nullptr);
        _prev->_next = this->_next;
        _next->_prev = this->_prev;
    }
    inline LinkedList* next() {
        return _next;
    }
    inline LinkedList* prev() {
        return _prev;
    }
    virtual void oops_do(OopClosure*) {};
};

#endif