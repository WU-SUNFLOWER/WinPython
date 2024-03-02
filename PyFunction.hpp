#ifndef PyFunction_Hpp
#define PyFunction_Hpp

#include "PyObject.hpp"
#include "CodeObject.hpp"
#include "Map.hpp"

class PyFunction : public PyObject {
    friend class FunctionKlass;
    friend class FrameObject;
    friend class Interpreter;
private:
    CodeObject* funcCode;
    PyString* funcName;
    uint32_t flag;

    PyObjectMap* _globals;

public:
    PyFunction(CodeObject* codeObject);
    PyFunction(Klass* klass);

    void setGlobalMap(PyObjectMap* map) {
        _globals = map;
    }

    const PyString* getName() const {
        return funcName;
    }
    uint32_t getFlags() const {
        return flag;
    }
};
#endif