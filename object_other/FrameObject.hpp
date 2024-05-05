#ifndef FrameObject_Hpp
#define FrameObject_Hpp

#include "OopClosure.hpp"
#include "arrayList.hpp"
#include "CodeObject.hpp"
#include "map.hpp"
#include "PyFunction.hpp"
#include "Block.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "Stack.hpp"

class FrameObject {
    friend class Interpreter;
private:
    PyList* _stack;  // ����ʱջ
    Stack<Block>* _blockStack;  // ���ջ���ڴ�������Ƕ�׵Ľṹ

    PyList* _consts;  // �����б�
    PyList* _names;  // ȫ�ֱ��������б�
    PyList* _varNames;  // ������ֲ����������б�

    PyDict* _locals;  // �ֲ�������
    PyDict* _globals;  // ȫ�ֱ�����

    // �������±���ʵģ��ֲ������б�
    // �ñ����ں����������ݣ������溯�����ڵľֲ�����
    PyList* _fastLocals;
    
    // �ñ�����ά����ǰջ���������к���������free variable��
    // ��cell variable
    PyList* _cells;  

    FrameObject* _callerFrame;  // caller������ջ�壬���ڷ���caller����

    CodeObject* _codeObject;
    PyString* _byteCodes;
    size_t pc;  // ָ�������

    bool _isEntryFrame;  // ���ĳ��Pythonջ������C++�����������õģ��ñ�ǻᱻ��Ϊtrue

public:
    // �ù��캯����������Ϊ<module>����ջ��
    FrameObject(CodeObject* code);

    ~FrameObject();

    // �ù�����������һ���ͨ��PyFunction����ջ��
    static FrameObject* allocate(PyFunction* callee, FrameObject* callerFrame, bool isEntryFrame, PyList* args);

    uint8_t getOpCode();
    uint16_t getOpArgument();
    bool hasMoreCode();
    
    void pushToStack(PyObject* object);
    PyObject* popFromStack();

    PyObject* getTopInStack();

    bool isRootFrame() const;
    bool isEntryFrame() const;

    // GC��ؽӿ�
    void oops_do(OopClosure* closure);
};

#endif