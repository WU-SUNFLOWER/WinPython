#ifndef ListKlass_Hpp
#define ListKlass_Hpp

#include "Klass.hpp"
#include "PyList.hpp"

class ListKlass : public Klass {
private:
    ListKlass() {};
    static ListKlass* instance;
public:
    static ListKlass* getInstance() {
        if (instance == nullptr) {
            instance = new ListKlass();
        }
        return instance;
    }
};

#endif