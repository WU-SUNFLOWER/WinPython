#ifndef ArrayList_Hpp
#define ArrayList_Hpp

#include <cassert>
#include <cstdint>

class OopClosure;

template <typename T>
class ArrayList {
private:
    size_t length;  // ��������ЧԪ�صĸ���
    size_t capacity;  // ���鿪���ڴ���ܳ���
    T* ptr;
    T _defaultElem;
    void expand(size_t targetLength = 0);
public:
    ArrayList(T defaultElem, int64_t n = 8ll);
    ~ArrayList();
    // �������е����һ��λ�����Ԫ��
    void push(T elem);
    void insert(size_t index, T elem);
    void set(size_t index, T elem);
    // ��ȡָ��λ�õ�Ԫ��
    T get(size_t index);
    // �������������һ��λ�õ�Ԫ��
    T pop() {
        assert(length > 0);
        return ptr[--length];
    }
    // ��ȡ���������һ��λ�õ�Ԫ�أ�����������
    T top() {
        return ptr[length - 1];
    }
    // ɾ��ָ��λ�õ�Ԫ��
    void deleteByIndex(size_t index);

    size_t getLength() const {
        return length;
    }
    size_t getCapacity() const {
        return capacity;
    }
    // GC��ؽӿ�
    void* operator new(size_t size);
    void oops_do(OopClosure* closure);
};

#endif