#include "memory.h"

static unsigned int s_count = 0;

void IncrementMemoryCount()
{
    s_count++;
}

void DecrementMemoryCount()
{
    s_count--;
}

unsigned int GetMemoryCount()
{
    return s_count;
}