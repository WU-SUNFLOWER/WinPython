#ifndef CodeObject_Hpp
#define CodeObject_Hpp

#include "arrayList.hpp"
#include "PyString.hpp"
#include "PyObject.hpp"
#include "arrayList.hpp"
#include <cstdint>

class CodeObject : public PyObject {
private:
    int32_t _argCount;
    int32_t _nLocals;
    int32_t _stackSize;
    int32_t _flag;
    PyString* _byteCodes;
    ArrayList<PyObject*>* _consts;
    ArrayList<PyObject*>* _names;
    ArrayList<PyObject*>* _varNames;
    ArrayList<PyObject*>* _freeVars;
    ArrayList<PyObject*>* _callVars;
    PyString* _moduleName;
    PyString* _fileName;
    int32_t _lineno;
    PyString* _notable;
public:
    CodeObject(int32_t argCount, int32_t nLocals, int32_t stackSize, int32_t flag,
        PyString* byteCodes, ArrayList<PyObject*>* consts, ArrayList<PyObject*>* names,
        ArrayList<PyObject*>* varNames, ArrayList<PyObject*>* freeVars,
        ArrayList<PyObject*>* callVars, PyString* moduleName, PyString* fileName,
        int32_t lineno, PyString* notable
    ) :
        _argCount(argCount),
        _nLocals(nLocals),
        _stackSize(stackSize),
        _flag(flag),
        _byteCodes(byteCodes),
        _consts(consts),
        _names(names),
        _varNames(varNames),
        _freeVars(freeVars),
        _callVars(callVars),
        _moduleName(moduleName),
        _fileName(fileName),
        _lineno(lineno),
        _notable(notable) {};
    friend class Interpreter;
    virtual void print() const {};
    virtual PyObject* add(PyObject* other) const {
        return nullptr;
    };
};
#endif