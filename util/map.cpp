#include "map.hpp"
#include "PyObject.hpp"
#include "Universe.hpp"
#include "OopClosure.hpp"

template<typename KEY, typename VAL>
void* MapItem<KEY, VAL>::operator new[](size_t size) {
    return Universe::PyHeap->allocate(size);
}

template<typename KEY, typename VAL>
void Map<KEY, VAL>::expand() {
    START_COUNT_TEMP_OBJECTS;
    auto self = this;
    PUSH_TEMP_PYOBJECT_MAP(self);
    MapItem<KEY, VAL>* newPtr = new MapItem<KEY, VAL>[capacity <<= 1];
    for (size_t i = 0; i < length; ++i) {
        newPtr[i] = self->ptr[i];
    }
    self->ptr = newPtr;

    if (self->length > 3) {
        auto x = self->ptr[3].value->getKlass()->getSize();
    }

    END_COUNT_TEMP_OBJECTS;
}

template<>
Map<PyObject*, PyObject*>* Map<PyObject*, PyObject*>::createMap(
    PyObject* defaultElem
) {
    Handle<PyObject*> _defaultElem = defaultElem;

    START_COUNT_TEMP_OBJECTS;
    Map* map = new Map();
    PUSH_TEMP_PYOBJECT_MAP(map);
    
    map->capacity = 8;
    map->length = 0;
    map->_default = _defaultElem;

    auto addr = new MapItem<PyObject*, PyObject*>[map->capacity];
    map->ptr = addr;

    END_COUNT_TEMP_OBJECTS;
    return map;
}

template<typename KEY, typename VAL>
Map<KEY, VAL>* Map<KEY, VAL>::createMap(VAL defaultElem) {
    puts("Try to create map with unknown type");
    exit(-1);
}

template<typename KEY, typename VAL>
void* Map<KEY, VAL>::operator new(size_t size) {
    return Universe::PyHeap->allocate(size);
}

template<typename KEY, typename VAL>
void Map<KEY, VAL>::set(KEY key, VAL value) {
}

template<>
void Map<PyObject*, PyObject*>::set(PyObject* key, PyObject* value) {
    
    auto self = this;
    Handle<PyObject*> _key = key;
    Handle<PyObject*> _value = value;
    // 如果key值在哈希表中已出现过，则直接覆盖对应的value值
    size_t searchIdx = self->getIndex(_key);
    if (searchIdx != -1) {
        self->ptr[searchIdx].value = _value;
        return;
    }

    START_COUNT_TEMP_OBJECTS;
    PUSH_TEMP_PYOBJECT_MAP(self);

    // 空间不够要扩容
    if (self->length >= self->capacity) {
        self->expand();
    }
    // 放置新元素
    auto elem = MapItem<PyObject*, PyObject*>(_key, _value);
    self->ptr[self->length++] = elem;

    if (self->length > 3) {
        auto x = self->ptr[3].value->getKlass()->getSize();
    }

    END_COUNT_TEMP_OBJECTS;
}

template<typename KEY, typename VAL>
VAL Map<KEY, VAL>::remove(KEY key) {

}

template<>
PyObject* Map<PyObject*, PyObject*>::remove(PyObject* key) {
    size_t searchIdx = getIndex(key);
    PyObject* ret = _default;
    if (searchIdx != -1) {
        PyObject* ret = ptr[searchIdx].value;
        // 哈希表的元素排列顺序是无所谓的，这里不用位移
        ptr[searchIdx] = ptr[--length];
    }
    if (length > 3) {
        auto x = this->ptr[3].value->getKlass()->getSize();
    }
    return ret;
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
    return getIndex(key) != -1;
}

template<typename KEY, typename VAL>
KEY Map<KEY, VAL>::getKeyByIndex(size_t index) {
    return ptr[index].key;
}

template<typename KEY, typename VAL>
KEY Map<KEY, VAL>::getValueByIndex(size_t index) {
    return ptr[index].value;
}

template<>
size_t Map<PyObject*, PyObject*>::getIndex(PyObject* targetKey) {
    for (size_t i = 0; i < length; ++i) {
        auto curkey = ptr[i].key;
        bool cur_isInt = isPyInteger(curkey);
        bool target_isInt = isPyInteger(targetKey);
        if (
            cur_isInt && target_isInt && curkey == targetKey ||
            !cur_isInt && !target_isInt && curkey->equal(targetKey) == Universe::PyTrue
        ) {
            return i;
        }
    }
    return -1;
}

template<typename KEY, typename VAL>
size_t Map<KEY, VAL>::getIndex(KEY key) {
    return -1;
}

template<typename KEY, typename VAL>
void Map<KEY, VAL>::oops_do(OopClosure* closure) {
    fputs("Can't directly call Map<KEY, VAL>::oops_do(OopClosure* closure)", stderr);
    exit(-1);
}

template<>
void Map<PyObject*, PyObject*>::oops_do(OopClosure* closure) {

    if (ptr == nullptr) return;

    closure->do_raw_mem(reinterpret_cast<void**>(&ptr), 
        capacity * sizeof(MapItem<PyObject*, PyObject*>));

    if (length > 3) {
        auto x = ptr[3].value->getKlass()->getSize();
    }

    for (size_t i = 0; i < length; ++i) {
        closure->do_oop(&(ptr[i].key));
        closure->do_oop(&(ptr[i].value));
    }

    if (length > 3) {
        auto x = ptr[3].value->getKlass()->getSize();
    }
}

template class Map<PyObject*, PyObject*>;