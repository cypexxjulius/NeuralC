#include "memory.h"

static unsigned int s_count = 0;

void incrementCount()
{
    s_count++;
}

void decrementCount()
{
    s_count--;
}

unsigned int getMemoryCount()
{
    return s_count;
}