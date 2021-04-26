#include "memory.h"
#include "src/platform/platform.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/core/error.h"
#include "src/utils/types.h"

static i32 s_count = 0;

static inline void IncrementMemoryCount()
{
    s_count++;
}

static inline void DecrementMemoryCount()
{
    s_count--;
}

/*
Wrappers for Memory managment
*/

static inline void *MemAllocImpl(size_t size)
{
    void*temp = malloc((size)); 
    IncrementMemoryCount();
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void* MemCallocImpl(size_t count,size_t size)
{ 
    void*temp = calloc((count), (size)); 
    IncrementMemoryCount();
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void MemFreeImpl(void *pointer) 
{ 
    free(pointer); 
    DecrementMemoryCount();
};

static inline void *MemReallocImpl(void* pointer,size_t size)
{
    void*temp = realloc(pointer, size);
    Assert(!temp, "Memory Reallocation failed");
    return temp;
}


struct memoryPointers Memory = {
    .Alloc = MemAllocImpl, 
    .Calloc = MemCallocImpl, 
    .Free=MemFreeImpl, 
    .Realloc=MemReallocImpl, 
    .Copy = memcpy, 
    .Compare = memcmp,
    .Set = memset
};


unsigned int GetMemoryCount()
{
    return s_count;
}