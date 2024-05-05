#ifndef Hpp_Space
#define Hpp_Space
class Space {
    friend class Heap;
//private:
public:
    char* _base;
    char* _top;
    char* _end;
    size_t _size;
    size_t _capacity;
    
    Space(size_t size);
    ~Space();

public:
    void* allocate(size_t size);
    void clear();
    bool canAlloc(size_t size);
    bool hasObject(void* addr);
};

#endif