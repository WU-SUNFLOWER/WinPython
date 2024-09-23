#include "RangeKlass.hpp"
#include "ObjectKlass.hpp"
#include "PyTypeObject.hpp"
#include "Universe.hpp"
#include "PyDict.hpp"
#include "StringTable.hpp"
#include "nativeFunctions.hpp"
#include "interpreter.hpp"



RangeKlass* RangeKlass::instance = nullptr;
RangeIteratorKlass* RangeIteratorKlass::instance = nullptr;

RangeKlass::RangeKlass() {

}

void RangeKlass::initialize()
{
    PyDict* dict = PyDict::createDict();
    setKlassDict(dict);
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_range);
    addSuper(ObjectKlass::getInstance());
    orderSupers();

}



void RangeKlass::print(const PyObject* object, int flags) const {
    const PyRange* range = static_cast<const PyRange*>(object);
    printf("range(%ld, %ld, %ld)", range->getstart(), range->getend(), range->getstep());
}

PyObject* RangeKlass::getIter(PyObject* object) const {
    checkLegalPyObject(object, this);
    return new RangeIterator(static_cast<PyRange*>(object));
}

PyObject* RangeKlass::allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args) {

    assert(callable == getTypeObject());

    if (args->getLength() != 1 && args->getLength() != 2 && args->getLength() != 3) {
        printf("range expected at least 1 argument, got %lld\n", args->getLength());
        exit(-1);
    }

    int64_t start = 0, end = 0, step = 1;
    switch (args->getLength()) {
    case 1:
        if (!isPyInteger(args->get(0))) {
            printf("range() expected int argument\n");
            exit(-1);
        }
        end = toRawInteger(args->get(0));
        break;
    case 2:
        if (!isPyInteger(args->get(0)) || !isPyInteger(args->get(1))) {
            printf("range() expected int arguments\n");
            exit(-1);
        }
        start = toRawInteger(args->get(0));
        end = toRawInteger(args->get(1));
        break;
    case 3:
        if (!isPyInteger(args->get(0)) || !isPyInteger(args->get(1)) || !isPyInteger(args->get(2))) {
            printf("range() expected int arguments\n");
            exit(-1);
        }
        start = toRawInteger(args->get(0));
        end = toRawInteger(args->get(1));
        step = toRawInteger(args->get(2));
        break;
    }

    if (step == 0) {
        printf("range() arg 3 must not be zero\n");
        exit(-1);
    }

    // 创建新的范围对象并设置其属性
    Handle<PyRange*> range = new PyRange(start, end, step);

    // 绑定klass到新对象
    range->setKlass(this);

    // 返回新分配的实例
    return range;
}

RangeIteratorKlass::RangeIteratorKlass() {
    PyDict* dict = PyDict::createDict();

    dict->set(StringTable::str_next,
        PackNativeFunc(NativeFunction::range_iterator_next));

    setKlassDict(dict);
}


PyObject* RangeIteratorKlass::next(PyObject* object) const {
    checkLegalPyObject(object, this);
    RangeIterator* iter = static_cast<RangeIterator*>(object);
    PyRange* range = iter->getOwner();
    int count = iter->getCount();
    if (count < range->getlength()) {
        PyObject* nextElem = range->get(count);
        iter->increaseCount();
        return nextElem;
    }
    else {
        return nullptr;
    }
}

void RangeIteratorKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    RangeIterator* iter = static_cast<RangeIterator*>(object);
    closure->do_oop(reinterpret_cast<PyObject**>(&iter->_owner));
}
