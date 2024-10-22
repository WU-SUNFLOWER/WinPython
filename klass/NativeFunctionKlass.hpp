#ifndef NativeFunctionKlass_Hpp
#define NativeFunctionKlass_Hpp

#include "Klass.hpp"

class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass() {};
    static NativeFunctionKlass* instance;
public:
    static NativeFunctionKlass* getInstance() {
        if (instance == nullptr) {
            instance = new NativeFunctionKlass();
        }
        return instance;
    }

    virtual size_t getSize() override;

    virtual void initialize() override;

    virtual void print(const PyObject* lhs, int flags) const override;

    virtual void oops_do(OopClosure* closure, PyObject* object) override;
};
#endif