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
    PyList* _stack;  // ����ʱջ
    ArrayList<Block>* _blockStack;  // ���ջ���ڴ�������Ƕ�׵Ľṹ

    PyList* _consts;  // �����б�
    PyList* _names;  // ȫ�ֱ��������б�
    PyList* _varNames;  // ������ֲ����������б�

    PyObjectMap* _locals;  // �ֲ�������
    PyObjectMap* _globals;  // ȫ�ֱ�����

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

public:
    // �ù��캯����������Ϊ<module>����ջ��
    FrameObject(CodeObject* code);
    // �ù��캯������һ���ͨ��PyFunction����ջ��
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