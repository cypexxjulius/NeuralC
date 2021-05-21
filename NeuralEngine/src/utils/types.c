#include "types.h"
#include <stdlib.h>

#include "src/platform/memory.h"
#include <string.h>

float GetUnsignedFloat(float x)
{
    return (x < 0) ? -x : x;
}

int RoundFloat(float num)
{
    return (int)roundf(num); 
}