#ifndef ArrayList_Hpp
#define ArrayList_Hpp

#include <cassert>
#include <cstdint>

class OopClosure;

template <typename T>
class ArrayList {
    friend class PyList;
    friend class ListKlass;
private:
    size_t length = 0;  // 数组内有效元素的个数
    size_t capacity = 0;  // 数组开辟内存的总长度
    T* ptr = nullptr;
    T _defaultElem;
    
    ArrayList* expand(size_t targetLength = 0);
public:
    static ArrayList* createArrayList(T defaultElem, int64_t n = 8ll, bool isInMeta = false);
    
    // 向数组中的最后一个位置添加元素
    void push(T elem);
    void insert(size_t index, T elem);
    void set(size_t index, T elem);
    // 获取指定位置的元素
    T get(size_t index);
    // 弹出数组中最后一个位置的元素
    T pop() {
        assert(length > 0);
        return ptr[--length];
    }
    // 获取数组中最后一个位置的元素（但不弹出）
    T top() {
        return ptr[length - 1];
    }
    // 删除指定位置的元素
    void deleteByIndex(size_t index);

    size_t getLength() const {
        return length;
    }
    size_t getCapacity() const {
        return capacity;
    }
    // GC相关接口
    void* operator new(size_t size);
    void oops_do(OopClosure* closure);
};

#endif