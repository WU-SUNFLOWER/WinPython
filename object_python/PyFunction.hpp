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

    PyDict* _globals;
    
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

    // 枚举定义的本质是创建一个新的类型，并不是创建一个类的成员变量
    // 因此这里不用加static关键字
    enum CO_FLAGS {
        // 用于检测函数体是否接收扩展位置参数（*args）
        CO_VARARGS = 0x4,
        // 用于检测函数体是否接收扩展键参数（**kwargs）
        CO_VARKEYWORDS = 0x8,
        CO_GENERATOR = 0x20,
    };

    PyFunction(CodeObject* codeObject);
    PyFunction(Klass* klass);
    PyFunction(NativeFuncPointer nativeFunc);

    void setGlobalMap(PyDict* map) {
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