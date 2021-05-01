#include "memory.h"
#include "src/platform/platform.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/core/error.h"
#include "src/utils/types.h"
#include "src/core/debug.h"


#define PageSize 32
#define BufferSize 16000
#define LedgerLength BufferSize / sizeof(u16)

typedef byte Page[PageSize];

static Page MemoryBuffer[BufferSize];
static u16 BufferLedger[LedgerLength] = { 0 };



static void* AllocateMemoryArea(u16 pageSize)
{
    for(volatile u32 i = 0; i < LedgerLength; i++)
    {
        if(BufferLedger[i] < UINT16_MAX)
        {
            u16 freePageLength = 0;
            u16 startIndex = 0;
            for(u32 k = 0; i < BufferSize; k++)
            {
                if(k > 15)
                {
                    k = 0;
                    while(BufferLedger[++i] == UINT16_MAX);
                }

                if(IsBitSet(BufferLedger[i], k) == 0)
                {
                    if(freePageLength == 0)
                        startIndex = k;

                    freePageLength++;
                }else 
                    freePageLength = 0;    // If 1 Page was used reset the start index
                


                if(freePageLength == pageSize) // Enough free pages in a row were found
                {
                    u32 index = i * sizeof(u16) * 8 + startIndex;

                    for(u32 j = startIndex; j <  (u32)startIndex + pageSize; j++)
                        SetBit(BufferLedger[i], j);

                    MemoryBuffer[index][0] = (u8)pageSize;

                    return &MemoryBuffer[index][1];
                }
            }
        }
    }
    return NULL;

}

static int ReleaseMemoryArea(void * data)
{
    u32 startIndex = (u32)(uintptr_t)((Page *)((byte*)data - 1)  - MemoryBuffer);

    u32 length = *((byte *)data - 1);

    for(u32 i = startIndex; i < startIndex + length; i++)
        ClrBit(BufferLedger[(i - i % 16) / 16], i % 16);    // Clearing the bits from the ledger to signal that the pages are free again 

#ifdef NEURAL_DEBUG
    for(u32 i = 0; i < length; i++)
    {
        for(u32 j = 0; j < PageSize; j++)
        {
            MemoryBuffer[startIndex + i ][j] = 0;
        }
    }
#endif

    return 0;    
};

static int MemoryFree(void *buffer)
{
    return ReleaseMemoryArea(buffer);
}


static void* MemoryAllocate(size_t size)
{   
    size++;
    u32 pageSize = (u32)((size - size % 32) / 32 + 1);


#ifndef NEURAL_DEBUG
    return AllocateMemoryArea(pageSize);
#else 
    byte* buffer = AllocateMemoryArea(pageSize);
    for(u32 i = 0; i < pageSize * PageSize - 1; i++)
    {
        buffer[i] = 0;
    }
    return buffer;
#endif
}



static void* MemoryCalloc(size_t typesize, size_t count)
{
    u32 size = (u32)(count * typesize);
    size++;

    u32 pageSize = (size - size % 32) / 32 + 1;


    byte* buffer = AllocateMemoryArea(pageSize);
    for(u32 i = 0; i < pageSize * PageSize - 1; i++)
    {
        buffer[i] = 0;
    }
    return buffer;
}

static void* MemoryRealloc(void* buffer, size_t newSize)
{
    if(newSize <= PageSize - 1)
        return buffer;


    u32 startIndex = (u32)(uintptr_t)((Page *)((byte*)buffer - 1)  - MemoryBuffer);
    u32 length = *((byte *)buffer - 1);

    u32 SizeOfPages = ((u32)newSize - newSize % 32) / 32 + 1;

    u32 diff = SizeOfPages - length;
    if(diff <= 0)
        return buffer;

    u8 isFree = 0;
    for(u32 i = startIndex; i < startIndex + diff; i++)
    {
        isFree += IsBitSet(BufferLedger[(i - i % 16) / 16], i % 16); 
    }   

    if(isFree == 0)
    {
        for(u32 i = startIndex; i < startIndex + diff; i++)
        {
            isFree += SetBit(BufferLedger[(i - i % 16) / 16], i % 16); 
        }   
        *((byte *)buffer - 1) = (byte)newSize;
        return buffer;
    }
    

    void* newBuffer = MemoryAllocate(newSize);
    memcpy(newBuffer, buffer, length * PageSize - 1);
    MemoryFree(buffer);

    return newBuffer;

}


/*
API for Memory managment
*/

static inline void *MemAllocImpl(size_t size)
{
    // void*temp = MemoryAllocate(size);
    void* temp = malloc(size);
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void* MemCallocImpl(size_t count,size_t size)
{ 
    // void*temp = MemoryCalloc(count, size); 
    void* temp = calloc(count, size);
    Assert(!temp, "Memory Allocation failed");
    return temp;
};

static inline void MemFreeImpl(void *pointer) 
{ 
#if 0
#ifdef NEURAL_DEBUG
    if(!IsAllocated(pointer))
    {
        fprintf(stderr, "[ALLOC ERROR] Passed invalid Pointer to function %s\n", __FUNCTION__);
        Assert(1, "");
    }
#endif 
    if(MemoryFree(pointer) < 0)
        fprintf(stderr, "[ALLOC ERROR] Failed to Free Memory\n");
#endif 
    free(pointer);
};

static inline void *MemReallocImpl(void* pointer,size_t size)
{
#if 0
#ifdef NEURAL_DEBUG
    if(!IsAllocated(pointer))
    {
        fprintf(stderr, "[ALLOC ERROR] Passed invalid Pointer to function %s\n", __FUNCTION__);
        Assert(1, "");
    }
#endif 
    void*temp = MemoryRealloc(pointer, size);
#endif 
    void* temp = realloc(pointer, size);
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