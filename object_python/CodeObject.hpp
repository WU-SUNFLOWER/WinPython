#ifndef CodeObject_Hpp
#define CodeObject_Hpp

#include "PyList.hpp"
#include "PyString.hpp"
#include "PyObject.hpp"
#include "CodeKlass.hpp"
#include <cstdint>

class FrameObject;

class CodeObject : public PyObject {
    friend class CodeKlass;
    friend class Interpreter;
    friend class FrameObject;
    friend class PyFunction;
private:
    int32_t _argCount;
    int32_t _nLocals;
    int32_t _stackSize;
    int32_t _flag;
    PyString* _byteCodes;
    PyList* _consts;
    PyList* _names;  // 符号表（例如变量的名称、函数的名称、方法的名称...）
    PyList* _varNames;
    PyList* _freeVars;
    PyList* _cellVars;
    PyString* _name;
    PyString* _fileName;
    int32_t _lineno;
    PyString* _notable;
public:
    CodeObject(int32_t argCount, int32_t nLocals, int32_t stackSize, int32_t flag,
        PyString* byteCodes, PyList* consts, PyList* names,
        PyList* varNames, PyList* freeVars,
        PyList* cellVars, PyString* name, PyString* fileName,
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
        _notable(notable) 
    {
        PyObject::isInMeta = true;
        setKlass(CodeKlass::getInstance());
    };

    void* operator new(size_t size);
    void operator delete(void*);

};
#endif