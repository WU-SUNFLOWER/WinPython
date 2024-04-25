#include "PyFunction.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include <cstdlib>
#include <cstdio>

PyFunction::PyFunction(CodeObject* codeObject) {
    funcCode = codeObject;
    funcName = codeObject->_name;
    flag = codeObject->_flag;
    _globals = nullptr;
    _defaultArgs = nullptr;
    _nativeFunc = nullptr;
    _freevars = nullptr;
    // 别忘了初始化klass指针
    setKlass(FunctionKlass::getInstance());
}

PyFunction::PyFunction(Klass* klass) {
    funcCode = nullptr;
    funcName = nullptr;
    flag = 0;
    _globals = nullptr;
    _defaultArgs = nullptr;
    _nativeFunc = nullptr;
    _freevars = nullptr;
    setKlass(klass);
}

PyFunction::PyFunction(NativeFuncPointer nativeFunc) {
    funcCode = nullptr;
    funcName = nullptr;
    flag = 0;
    _globals = nullptr;
    _defaultArgs = nullptr;
    _nativeFunc = nativeFunc;
    _freevars = nullptr;
    setKlass(NativeFunctionKlass::getInstance());
}

// 为函数对象挂载默认参数
void PyFunction::setDefaultArgs(PyList* args) {
    _defaultArgs = args;
}

PyObject* PyFunction::callNativeFunc(PyList* args) const {
    if (getKlass() != NativeFunctionKlass::getInstance()) {
        printf("You can only call PyFunction::callNativeFunc for native function!");
        exit(-1);
    }
    return (*_nativeFunc)(args);
}
