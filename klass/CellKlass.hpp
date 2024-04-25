#ifndef Cell_Klass_Hpp
#define Cell_Klass_Hpp

#include "Klass.hpp"

class CellKlass : public Klass {
private:
    CellKlass() {};
    static CellKlass* instance;
public:
    static CellKlass* getInstance() {
        if (instance == nullptr) {
            instance = new CellKlass();
        }
        return instance;
    }
};

#endif