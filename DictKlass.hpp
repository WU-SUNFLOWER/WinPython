#ifndef Hpp_DictKlass
#define Hpp_DictKlass

#include "Klass.hpp"

class DictKlass: public Klass {
private:
    DictKlass() {};
    static DictKlass* instance;
public:
    static DictKlass* getInstance() {
        if (instance == nullptr) {
            instance = new DictKlass();
        }
        return instance;
    }

    virtual void initialize() override;

    virtual void print(const PyObject* object, int flags) const override;
    virtual void delete_subscr(PyObject* object, 
        PyObject* subscription) const override;
};

enum DictIterType {Iter_Keys, Iter_Values, Iter_Items};

template<DictIterType type>
class DictIteratorKlass : public Klass {
private:
    static DictIteratorKlass* instance;
    DictIteratorKlass() {};
public:
    static DictIteratorKlass* getInstance() {
        if (instance == nullptr) {
            instance = new DictIteratorKlass();
        }
        return instance;
    }
    virtual PyObject* getIter(PyObject* object) const override {
        return object;
    };
    virtual PyObject* next(PyObject* object) const override;
};

#endif