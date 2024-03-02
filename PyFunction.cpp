#include "PyFunction.hpp"
#include "FunctionKlass.hpp"

PyFunction::PyFunction(CodeObject* codeObject) {
    funcCode = codeObject;
    funcName = codeObject->_name;
    flag = codeObject->_flag;
    _globals = nullptr;
    // �����˳�ʼ��klassָ��
    setKlass(FunctionKlass::getInstance());
}

PyFunction::PyFunction(Klass* klass) {
    funcCode = nullptr;
    funcName = nullptr;
    flag = 0;
    _globals = nullptr;
    setKlass(klass);
}