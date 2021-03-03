#ifndef __N_TYPES_H_
#define __N_TYPES_H_

#include <stdint.h>
#include <inttypes.h>


typedef uint16_t ui16;
typedef int16_t i16;
typedef int8_t byte;

typedef float float2[2]; 


typedef struct 
{
    int x;
    int y;
}pos2D;


#endif // __N_TYPES_H_