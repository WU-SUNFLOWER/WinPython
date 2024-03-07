#ifndef CodeObject_Hpp
#define CodeObject_Hpp

#include "arrayList.hpp"
#include "PyString.hpp"
#include "PyObject.hpp"
#include <cstdint>

class FrameObject;

class CodeObject : public PyObject {
private:
    int32_t _argCount;
    int32_t _nLocals;
    int32_t _stackSize;
    int32_t _flag;
    PyString* _byteCodes;
    PyObjectList* _consts;
    PyObjectList* _names;  // 符号表（例如变量的名称、函数的名称、方法的名称...）
    PyObjectList* _varNames;
    PyObjectList* _freeVars;
    PyObjectList* _cellVars;
    PyString* _name;
    PyString* _fileName;
    int32_t _lineno;
    PyString* _notable;
public:
    CodeObject(int32_t argCount, int32_t nLocals, int32_t stackSize, int32_t flag,
        PyString* byteCodes, PyObjectList* consts, PyObjectList* names,
        PyObjectList* varNames, PyObjectList* freeVars,
        PyObjectList* cellVars, PyString* name, PyString* fileName,
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
        _cellVars(cellVars),
        _name(name),
        _fileName(fileName),
        _lineno(lineno),
        _notable(notable) {};
    
    friend class Interpreter;
    friend class FrameObject;
    friend class PyFunction;

    virtual void print() const {};
    virtual PyObject* add(PyObject* other) const {
        return nullptr;
    };
};
#endif