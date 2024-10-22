#ifndef Interpreter_Hpp
#define Interpreter_Hpp

#include "Stack.hpp"
#include "CodeObject.hpp"
#include "PyInteger.hpp"
#include "PyFunction.hpp"
#include "Universe.hpp"
#include "bytecode.hpp"
#include "Map.hpp"
#include "PyDict.hpp"
#include "OopClosure.hpp"
#include "Handle.hpp"

class Interpreter {
    enum Status {
        IS_OK,
        IS_BREAK,
        IS_CONTINUE,
        IS_EXCEPTION,
        IS_RETURN,
        IS_YIELD,
    };

private:
    Interpreter();
    static Interpreter* instance;

    Status _status;

    PyDict* _builtins;
    FrameObject* _curFrame;
    PyObject* _retValue;  // 该成员变量用于在切换栈桢上下文时，缓存callee的返回值

public:
    
    // Interpreter也采用单例模式，方便C++代码中有需要随时访问解释器实例对象！
    static Interpreter* getInstance() {
        if (instance == nullptr) {
            instance = new Interpreter();
        }
        return instance;
    }

    void run(CodeObject* code);

    void evalFrame();

    void compareTwoPythonObjects(Handle<PyObject*> lhs, Handle<PyObject*> rhs, uint16_t);

    void makeFunction(int16_t defaultArgCount, bool isClosureFunc);

    void entryIntoNewFrame(
        Handle<PyObject*> callableObject, 
        Handle<PyList*> rawArgs, 
        uint8_t argNumber_pos, uint8_t argNumber_kw);

    void destroyCurFrame();
    void exitFromCurFrame();

    PyObject* callVirtual(Handle<PyObject*> callable, Handle<PyList*> args);

    FrameObject* getCurrentFrame();

    // GC相关接口
    void oops_do(OopClosure* closure);
};

#endif