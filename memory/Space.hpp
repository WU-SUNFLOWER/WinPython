#ifndef Hpp_Space
#define Hpp_Space

#define Alignment_Mask (7)
#define Is_Valid_Space_Addr(x) (((uint64_t)(x) & (Alignment_Mask)) == 0)

class Space {
    friend class Heap;
private:
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