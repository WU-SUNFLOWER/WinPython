#ifndef FrameObject_Hpp
#define FrameObject_Hpp

#include "arrayList.hpp"
#include "CodeObject.hpp"
#include "map.hpp"
#include "PyFunction.hpp"
#include "Block.hpp"
#include "PyList.hpp"

class FrameObject {
private:
    PyList* _stack;  // 运行时栈
    ArrayList<Block>* _blockStack;  // 这个栈用于处理代码块嵌套的结构

    PyList* _consts;  // 常量列表
    PyList* _names;  // 全局变量名称列表
    PyList* _varNames;  // 函数体局部变量名称列表

    PyObjectMap* _locals;  // 局部变量表
    PyObjectMap* _globals;  // 全局变量表

    // （基于下标访问的）局部变量列表
    // 该表用于函数参数传递，及储存函数体内的局部变量
    PyList* _fastLocals;
    
    // 该表用于维护当前栈桢正在运行函数的所有free variable，
    // 和cell variable
    PyList* _cells;  

    FrameObject* _callerFrame;  // caller函数的栈桢，用于返回caller函数

    CodeObject* _codeObject;
    PyString* _byteCodes;
    size_t pc;  // 指令计数器

public:
    // 该构造函数仅适用于为<module>创建栈桢
    FrameObject(CodeObject* code);
    // 该构造函数用于一般的通过PyFunction创建栈桢
    FrameObject(PyFunction* callee, FrameObject* callerFrame, PyList* args);
    ~FrameObject();

    uint8_t getOpCode();
    uint16_t getOpArgument();
    bool hasMoreCode();
    
    void pushToStack(PyObject* object);
    PyObject* popFromStack();

    PyObject* getTopInStack();

    bool isRootFrame() const;

    friend class Interpreter;
};

#endif