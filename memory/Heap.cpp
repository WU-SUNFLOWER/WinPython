#include "Heap.hpp"
#include <cstdio>
#include "ScavengeOopClosure.hpp"

Heap* Heap::instance = nullptr;
size_t Heap::MaxCapacity = 1 * 1024 * 1024;

Heap* Heap::getInstance() {
    if (instance == nullptr) {
        instance = new Heap(MaxCapacity);
    }
    return instance;
}

Heap::Heap(size_t size) {
    mem_1 = new Space(size);
    mem_2 = new Space(size);
    metaspace = new Space(size >> 4);

    mem_1->clear();
    mem_2->clear();
    metaspace->clear();

    eden = mem_1;
    survivor = mem_2;
}

Heap::~Heap() {
    if (mem_1) delete mem_1;
    if (mem_2) delete mem_2;
    if (metaspace) delete metaspace;
    mem_1 = nullptr;
    mem_2 = nullptr;
    eden = nullptr;
    survivor = nullptr;
}

void* Heap::allocate(size_t size) {
    if (!eden->canAlloc(size)) {
        gc();
        if (!eden->canAlloc(size)) {
            fputs("eden space hasn't enough space to allocate.", stderr);
            exit(-1);
        }
    }
    return eden->allocate(size);
}

void* Heap::allocateMeta(size_t size) {
    if (!metaspace->canAlloc(size)) {
        puts("meta space hasn't enough space to allocate.");
        exit(-1);
    }
    return metaspace->allocate(size);
}

void Heap::copyLiveObjects() {
    ScavengeOopClosure closure(eden, survivor, metaspace);
    closure.scavenge();
}

void Heap::gc() {

    static int count = 0;
    count++;

    puts("gc starting...");
    puts("  before gc:");
    printf("  eden's capacity is %llu\n", eden->_capacity);
  
    // 将eden中的幸存对象悉数转移到survivor中
    copyLiveObjects();

    // 将原先的survivor空间作为新的eden空间，供后来的新生对象分配空间
    // 将原先的eden空间清空，用作下一次的survivor空间
    Space* temp = eden;
    eden = survivor;
    survivor = temp;
    survivor->clear();

    puts("  after gc:");
    printf("  eden's capacity is %llu\n", eden->_capacity);
    puts("gc end");
}
