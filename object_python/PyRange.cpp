#include "PyRange.hpp"
#include "RangeKlass.hpp"
#include "Universe.hpp"
#include "PyInteger.hpp"


PyRange::PyRange(long start, long end, long step) :
    _start(start), _end(end), _step(step) {
    if (step > 0)
        _length = (end - start + step - 1) / step;
    else if (step < 0)
        _length = (start - end - step - 1) / (-step);
    else
        _length = 0;

    setKlass(RangeKlass::getInstance());
}

PyRange* PyRange::createRange(long start, long end, long step) {
    return new PyRange(start, end, step);
}

PyObject* PyRange::get(size_t index) const {
    if (index >= _length) {
        return Universe::PyNone;
    }
    return toPyInteger(_start + index * _step);
}



RangeIterator::RangeIterator(PyRange* owner)
    : _count(0), _owner(owner)
{
    setKlass(RangeIteratorKlass::getInstance());
}
