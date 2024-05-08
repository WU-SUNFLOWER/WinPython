#ifndef Map_Hpp
#define Map_Hpp

#include <cstdint>

class OopClosure;

// 哈希表成员对象
template<typename KEY, typename VAL>
class MapItem {
public:
    KEY key;
    VAL value;

    void* operator new[](size_t size);
    
    // 无参构造函数
    MapItem() : key(0), value(0) {};
    // 普通构造函数
    MapItem(KEY key, VAL value) : key(key), value(value) {};
    // 拷贝构造函数
    MapItem(const MapItem<KEY, VAL>& other) : key(other.key), value(other.value) {};

};

template<typename KEY, typename VAL>
class Map {
private:
    VAL _default;
    size_t capacity = 0;
    size_t length = 0;
    MapItem<KEY, VAL>* ptr = nullptr;
    void expand();
public:
    static Map* createMap(VAL defaultElem);
    
    void* operator new(size_t size);

    // 哈希表的增删改查基础操作
    void set(KEY key, VAL value);
    VAL remove(KEY key);
    VAL get(KEY key);
    bool has(KEY key);

    
    // 根据哈希表数组真实下标，反查key值和value值
    KEY getKeyByIndex(size_t index);
    KEY getValueByIndex(size_t index);
    // 根据key值，计算哈希表数组下标
    size_t getIndex(KEY key);

    MapItem<KEY, VAL>* getPtr() const {
        return ptr;
    }

    size_t getLength() const {
        return length;
    }
    size_t getCapacity() const {
        return capacity;
    }

    void oops_do(OopClosure* closure);

};
#endif