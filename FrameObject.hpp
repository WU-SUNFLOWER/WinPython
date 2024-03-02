#ifndef FrameObject_Hpp
#define FrameObject_Hpp

#include "arrayList.hpp"
#include "CodeObject.hpp"
#include "map.hpp"
#include "PyFunction.hpp"
#include "Block.hpp"

class FrameObject {
private:
    PyObjectList* _stack;  // ����ʱջ
    ArrayList<Block>* _blockStack;  // ���ջ���ڴ�������Ƕ�׵Ľṹ

    PyObjectList* _consts;  // �����б�
    PyObjectList* _names;  // ���������б�

    PyObjectMap* _locals;  // �ֲ�������
    PyObjectMap* _globals;  // ȫ�ֱ�����
    // �������±���ʵģ��ֲ�������
    // �ñ����ں����������ݣ������溯�����ڵľֲ�����
    PyObjectList* _fastLocals;

    FrameObject* _callerFrame;  // caller������ջ�壬���ڷ���caller����

    CodeObject* _codeObject;
    PyString* _byteCodes;
    size_t pc;  // ָ�������

public:
    // �ù��캯����������Ϊ<module>����ջ��
    FrameObject(CodeObject* code);
    // �ù��캯������һ���ͨ��PyFunction����ջ��
    FrameObject(PyFunction* callee, FrameObject* callerFrame, PyObjectList* args);
    ~FrameObject();

    uint8_t getOpCode();
    uint16_t getOpArgument();
    bool hasMoreCode();
    
    void pushToStack(PyObject* object);
    PyObject* popFromStack();

    bool isRootFrame() const;

    friend class Interpreter;
};

#endif