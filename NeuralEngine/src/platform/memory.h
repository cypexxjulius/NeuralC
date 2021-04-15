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

static struct 
{
    void*(*Alloc)(size_t size);
    void*(*Calloc)(size_t count, size_t typesize);
    void (*Free)(void* pointer);
    void*(*Realloc)(void* pointer, size_t size);
    void*(*Copy)(void* Dest, void* Src, size_t count);
    int(*Compare)(const void* Buffer0, const void* Buffer1, size_t Count);
} 
Memory = 
{
    .Alloc = MemAllocImpl, 
    .Calloc = MemCallocImpl, 
    .Free=MemFreeImpl, 
    .Realloc=MemReallocImpl, 
    .Copy = memcpy, 
    .Compare = memcmp
};

/*
Creating New Object
*/

#define CreateObject(objectName) Memory.Calloc(1, sizeof(objectName))

#endif // __MEMORY_H_