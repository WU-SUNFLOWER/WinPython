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
            instance = new FunctionKlass();
        }
        return instance;
    }

    virtual size_t getSize() override;

    virtual void initialize() override;

    virtual void print(const PyObject* lhs, int flags) const override;

    virtual void oops_do(OopClosure* closure, PyObject* object) override;
};
#endif