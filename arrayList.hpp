#ifndef ArrayList_Hpp
#define ArrayList_Hpp

#include <cassert>

template <typename T>
class ArrayList {
private:
    size_t length;  // 数组内有效元素的个数
    size_t capacity;  // 数组开辟内存的总长度
    T* ptr;
    T _defaultElem;
    void expand();
public:
    ArrayList(T defaultElem, size_t n = 8);
    ~ArrayList();
    // 向数组中的最后一个位置添加元素
    void push(T elem);
    void insert(size_t index, T elem);
    void set(size_t index, T elem);
    // 获取指定位置的元素
    T get(size_t index);
    // 获取数组中最后一个位置的元素
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