#ifndef FunctionKlass_Hpp
#define FunctionKlass_Hpp

#include "Klass.hpp"

class FunctionKlass : public Klass {
private:
    FunctionKlass() {};
    static FunctionKlass* instance;
public:
    static FunctionKlass* getInstance() {
        if (instance == nullptr) {
            return new FunctionKlass();
        }
        return instance;
    }

    virtual void print(const PyObject* lhs) const override;
};
#endif