#include "map.hpp"
#include "PyObject.hpp"
#include "Universe.hpp"

template<typename KEY, typename VAL>
void Map<KEY, VAL>::expand() {
    MapItem<KEY, VAL>* newPtr = new MapItem<KEY, VAL>[capacity <<= 1];
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = ptr[i];
    }
    delete[] ptr;
    ptr = newPtr;
}

template<typename KEY, typename VAL>
Map<KEY, VAL>::Map(VAL defaultValue) {
    capacity = 8;
    length = 0;
    _default = defaultValue;
    ptr = new MapItem<KEY, VAL>[capacity];
}

template<typename KEY, typename VAL>
Map<KEY, VAL>::~Map() {
    delete[] ptr;
}

template<typename KEY, typename VAL>
void Map<KEY, VAL>::set(KEY key, VAL value) {
    // 如果key值在哈希表中已出现过，则直接覆盖对应的value值
    size_t searchIdx = getIndex(key);
    if (getIndex(key) != -1) {
        ptr[searchIdx].value = value;
        return;
    }
    // 空间不够要扩容
    if (length >= capacity) expand();
    // 放置新元素
    ptr[length++] = MapItem<KEY, VAL>(key, value);
}

template<typename KEY, typename VAL>
VAL Map<KEY, VAL>::remove(KEY key) {
    size_t searchIdx = getIndex(key);
    if (searchIdx != -1) {
        VAL ret = ptr[searchIdx].value;
        // 哈希表的元素排列顺序是无所谓的，这里不用位移
        ptr[searchIdx] = ptr[--length];
        return ret;
    }
    else {
        return _default;
    }
}

template<typename KEY, typename VAL>
VAL Map<KEY, VAL>::get(KEY key) {
    size_t searchIdx = getIndex(key);
    size_t failFlag = -1;
    if (searchIdx != failFlag) {
        return ptr[searchIdx].value;
    }
    else {
        return _default;
    }
}

template<typename KEY, typename VAL>
bool Map<KEY, VAL>::has(KEY key) {
    return getIndex(key) >= 0;
}

template<typename KEY, typename VAL>
KEY Map<KEY, VAL>::getKeyByIndex(size_t index) {
    return ptr[index].key;
}

template<typename KEY, typename VAL>
KEY Map<KEY, VAL>::getValueByIndex(size_t index) {
    return ptr[index].value;
}

template<typename KEY, typename VAL>
size_t Map<KEY, VAL>::getIndex(KEY& key) {
    for (size_t i = 0; i < length; ++i) {
        if (ptr[i].key->equal(key) == Universe::PyTrue) {
            return i;
        }
    }
    return -1;
}

template class Map<PyObject*, PyObject*>;