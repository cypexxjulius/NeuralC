#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdlib.h>
#include <stdint.h>
#include "src/utils/types.h"


struct memoryPointers
{
    void*(*Alloc)(size_t size);
    void*(*Calloc)(size_t count, size_t typesize);
    void (*Free)(void* pointer);
    void*(*Realloc)(void* pointer, size_t size);
    void*(*Copy)(void* Dest, const void* Src, size_t count);
    int(*Compare)(const void* Buffer0, const void* Buffer1, size_t Count);
    void*(*Set)(void*Buffer, int Value, size_t BufferSize);
};

extern struct memoryPointers Memory;

extern unsigned int GetMemoryCount();

/*
Creating New Object
*/

#define CreateObject(objectName) Memory.Calloc(1, sizeof(objectName))

#endif // __MEMORY_H_