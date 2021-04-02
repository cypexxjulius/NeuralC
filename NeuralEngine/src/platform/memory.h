#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/core/error.h"

void IncrementMemoryCount();

void DecrementMemoryCount();

unsigned int GetMemoryCount();

/*
Wrappers for Memory managment
*/

static inline void *MemAlloc(size_t size)
{
    void*temp = malloc((size)); 
    IncrementMemoryCount();
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void* MemCalloc(size_t count,size_t size)
{ 
    void*temp = calloc((count), (size)); 
    IncrementMemoryCount();
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void MemFree(void *pointer) 
{ 
    free(pointer); 
    DecrementMemoryCount();
};

static inline void *MemRealloc(void* pointer,size_t size)
{
    void*temp = realloc(pointer, size);
    Assert(!temp, "Memory Reallocation failed");
    return temp;
}
// Move or copy memory

#define MemCpy(dest, src, count) memcpy(dest, src, count) 

// Compare memory

#define MemCmp(dest, src, count) memcmp(dest, src, count)

/*
Creating New Object
*/

#define CreateObject(objectName) MemCalloc(1, sizeof(objectName))

#endif // __MEMORY_H_