#ifndef PyFunction_Hpp
#define PyFunction_Hpp

#include "PyObject.hpp"
#include "CodeObject.hpp"
#include "Map.hpp"
#include "PyList.hpp"

// ����C++�ڽ���������ָ��
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
        ���溯����Ĭ�ϲ���
        Python�й涨�����ķ�Ĭ�ϲ�����������Ĭ�ϲ���֮ǰ
        ���磺
        def foo(a, b=1) �Ϸ�
        def foo(a=1, b) ����
    */
    PyList* _defaultArgs;

    /* 
        ����б���ΪPython�к����հ������Է���ġ�
        �����ڴ��汻Ƕ�׺�����inner��
        �԰������ĺ�����outer���ľֲ�������������� 
    */
    PyList* _freevars;

    // ���ָ��ΪC++�ڽ�����Ԥ������ͨPython����Ĭ��Ϊ��
    NativeFuncPointer _nativeFunc;

public:

    // ö�ٶ���ı����Ǵ���һ���µ����ͣ������Ǵ���һ����ĳ�Ա����
    // ������ﲻ�ü�static�ؼ���
    enum CO_FLAGS {
        // ���ڼ�⺯�����Ƿ������չλ�ò�����*args��
        CO_VARARGS = 0x4,
        // ���ڼ�⺯�����Ƿ������չ��������**kwargs��
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