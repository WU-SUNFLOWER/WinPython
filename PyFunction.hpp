#ifndef PyFunction_Hpp
#define PyFunction_Hpp

#include "PyObject.hpp"
#include "CodeObject.hpp"
#include "Map.hpp"
#include "PyList.hpp"

// 定义C++内建函数调用指针
typedef PyObject* (*NativeFuncPointer)(PyList* args);

class PyFunction : public PyObject {
    friend class FunctionKlass;
    friend class NativeFunctionKlass;
    friend class FrameObject;
    friend class Interpreter;
private:
    CodeObject* funcCode;
    PyString* funcName;
    uint32_t flag;

    PyObjectMap* _globals;
    
    /*
        储存函数的默认参数
        Python中规定函数的非默认参数，必须在默认参数之前
        例如：
        def foo(a, b=1) 合法
        def foo(a=1, b) 报错
    */
    PyList* _defaultArgs;

    /* 
        这个列表是为Python中函数闭包的特性服务的。
        它用于储存被嵌套函数（inner）
        对包裹它的函数（outer）的局部变量的依赖情况 
    */
    PyList* _freevars;

    // 这个指针为C++内建函数预留，普通Python函数默认为空
    NativeFuncPointer _nativeFunc;

public:
    PyFunction(CodeObject* codeObject);
    PyFunction(Klass* klass);
    PyFunction(NativeFuncPointer nativeFunc);

    void setGlobalMap(PyObjectMap* map) {
        _globals = map;
    }
    void setDefaultArgs(PyList* args);
    void setFreevars(PyList* cells) {
        _freevars = cells;
    }

    const PyString* getName() const {
        return funcName;
    }
    uint32_t getFlags() const {
        return flag;
    }

    PyObject* callNativeFunc(PyList* args) const;
};
#endif