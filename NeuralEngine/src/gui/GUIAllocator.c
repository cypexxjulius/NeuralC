#include "GUIAllocator.h"
#include "src/core/error.h"

#define MEMORY_SIZE 2048

static u32 GUIMemoryCount;

static u8 GUIMemory[MEMORY_SIZE];

void GUIAllocatorReset()
{
    GUIMemoryCount = 0;

    for(u32 i = 0; i < MEMORY_SIZE; i++)
        GUIMemory[i] = 0;

}

void* GUIAlloca(u16 size)
{
    Assert(GUIMemoryCount + size > MEMORY_SIZE, "Allocator overflowed");

    void* temp = GUIMemory + GUIMemoryCount;
    GUIMemoryCount += size;

    return temp;
}



