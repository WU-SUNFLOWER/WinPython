#ifndef Hpp_DictKlass
#define Hpp_DictKlass

#include "Klass.hpp"
#include "PyDict.hpp"

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

    virtual size_t getSize() override {
        return sizeof(PyDict);
    }

    virtual void initialize() override;

    virtual void print(const PyObject* object, int flags) const override;
    
    virtual PyObject* len(const PyObject* object) const override;

    // 读取下标
    virtual PyObject* subscr(PyObject* object, PyObject* subscrption) const override;
    // 设置下标
    virtual void store_subscr(PyObject* object, PyObject* subscription, PyObject* newObject) const override;
    // 删除下标指定的元素
    virtual void delete_subscr(PyObject* object, 
        PyObject* subscription) const override;

    virtual void oops_do(OopClosure* closure, PyObject* object) override;
    virtual PyObject* isBoolTrue(PyObject* object) override;
};

enum DictIterType {Iter_Keys, Iter_Values, Iter_Items};

template<DictIterType type>
class DictIteratorKlass : public Klass {
private:
    static DictIteratorKlass* instance;
    DictIteratorKlass();
public:
    static DictIteratorKlass* getInstance() {
        if (instance == nullptr) {
            instance = new DictIteratorKlass();
        }
        return instance;
    }
    virtual size_t getSize() override;
    virtual PyObject* getIter(PyObject* object) const override {
        return object;
    };
    virtual PyObject* next(PyObject* object) const override; 
    
    virtual void oops_do(OopClosure* closure, PyObject* object) override;
    
};

#endif