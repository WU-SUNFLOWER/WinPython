#include "Space.hpp"
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>

Space::Space(size_t size) {
    _size = size;
    _base = (char*)malloc(size);
    _end = _base + size;
    // 保证地址空间首地址按16字节对齐
    _top = (char*)(reinterpret_cast<uint64_t>(_base + 15) & ~15);
    _capacity = _end - _top;
    if (_top == nullptr) {
        puts("Don't have enough memory space.");
        exit(-1);
    }
}

Space::~Space() {
    if (_base) {
        free(_base);
        _base = 0;
    }
    _top = 0;
    _end = 0;
    _size = 0;
    _capacity = 0;
}

void* Space::allocate(size_t size) {
    size = (size + 7) & ~7;
    char* start = _top;
    _top += size;
    _capacity -= size;
    return start;
}

void Space::clear() {
    memset(_base, 0x00, _size);
    // 将_top和_capacity复位
    _top = (char*)(reinterpret_cast<uint64_t>(_base + 15) & ~15);
    _capacity = _end - _top;
}

bool Space::canAlloc(size_t size) {
    return size < _capacity;
}

bool Space::hasObject(char* addr) {
    return _base <= addr && addr < _end;
}
