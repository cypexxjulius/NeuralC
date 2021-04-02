#include "types.h"
#include <stdlib.h>

#include "src/platform/memory.h"
#include <string.h>

void mat4print(mat4s mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int k = 0; k < 4; k++)
        {
            printf("%f ", mat.raw[i][k]);
        }
        puts("");
    }
    puts("\n");
}

float GetUnsignedFloat(float x)
{
    return (x < 0) ? -x : x;
}

int RoundFloat(float num)
{
    return (int)roundf(num); 
}