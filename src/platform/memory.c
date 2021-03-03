#include "memory.h"

static i16 s_count = 0;

void incrementCount()
{
    s_count++;
}

void decrementCount()
{
    s_count--;
}

i16 getMemoryCount()
{
    return s_count;
}