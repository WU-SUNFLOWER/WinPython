#ifndef Hpp_Heap
#define Hpp_Heap

#include "Space.hpp"

class Heap {
private:
    Space* mem_1;
    Space* mem_2;

    Space* eden;
    Space* survivor;

    Space* metaspace;

    Heap(size_t size);

public:
    static size_t MaxCapacity;
    static Heap* instance;
    static Heap* getInstance();
    
    ~Heap();

    void* allocate(size_t size);
    void* allocateMeta(size_t size);
    void copyLiveObjects();

    void gc();
};

#endif