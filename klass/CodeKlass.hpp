#ifndef Hpp_CodeKlass
#define Hpp_CodeKlass

#include "Klass.hpp"

class CodeKlass : public Klass {
private:
    static CodeKlass* instance;
    CodeKlass();
public:
    static CodeKlass* getInstance() {
        if (instance == nullptr) {
            instance = new CodeKlass();
        }
        return instance;
    }
    virtual void oops_do(OopClosure* closure, PyObject* object) override;
    virtual size_t getSize() override;
};

#endif