#ifndef PYRANGE_HPP
#define PYRANGE_HPP

#include "PyObject.hpp"

class PyRange : public PyObject {
    friend class RangeKlass;
private:
    long _start;
    long _end;
    long _step;
    size_t _length;
public:
    PyRange(long start, long end, long step);
    static PyRange* createRange(long start, long end, long step);

    long getstart() const { return _start; }
    long getend() const { return _end; }
    long getstep() const { return _step; }
    size_t getlength() const { return _length; }

    PyObject* get(size_t index) const;
};

class RangeIterator : public PyObject {
    friend class RangeIteratorKlass;
private:
    PyRange* _owner;
    int _count;
public:
    RangeIterator(PyRange* owner);
    PyRange* getOwner() {
        return _owner;
    }
    int getCount() {
        return _count;
    }
    void increaseCount() {
        ++_count;
    }
};

#endif
