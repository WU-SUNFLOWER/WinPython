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

    virtual void print(const PyObject* lhs) const override;
};
#endif