#ifndef MethodKlass_Hpp
#define MethodKlass_Hpp

#include "Klass.hpp"

class MethodKlass : public Klass {
private:
    MethodKlass() {};
    static MethodKlass* instance;
public:
    static MethodKlass* getInstance() {
        if (instance == nullptr) {
            instance = new MethodKlass();
        }
        return instance;
    }

    virtual void print(const PyObject* lhs, int flags = 0) const override;
    virtual void initialize() override;
};
#endif