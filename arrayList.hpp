#ifndef ArrayList_Hpp
#define ArrayList_Hpp

#include <cassert>

template <typename T>
class ArrayList {
private:
    size_t length;  // ��������ЧԪ�صĸ���
    size_t capacity;  // ���鿪���ڴ���ܳ���
    T* ptr;
    T _defaultElem;
    void expand();
public:
    ArrayList(T defaultElem, size_t n = 8);
    ~ArrayList();
    // �������е����һ��λ�����Ԫ��
    void push(T elem);
    void insert(size_t index, T elem);
    void set(size_t index, T elem);
    // ��ȡָ��λ�õ�Ԫ��
    T get(size_t index);
    // ��ȡ���������һ��λ�õ�Ԫ��
    T pop() {
        assert(length > 0);
        return ptr[--length];
    }

    size_t getLength() const {
        return length;
    }
    size_t getCapacity() const {
        return capacity;
    }

};

#endif