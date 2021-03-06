#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>
#include <cglm/struct.h>
#include <stdbool.h>
#include <math.h>

#include "datatypes/Vector.h"
#include "datatypes/String.h"

typedef uint8_t byte;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef float_t f32;


#define persist static

#define local static

#define vec3s(x, y, z)       (vec3s){{x, y, z}}
#define vec4s(x, y, z, w)    (vec4s){{x, y, z, w}}

#define StringLiteralLength(string) (sizeof(string) - 1)

typedef struct v2 { 
    union {
        struct { float x,y; };
        struct { float width, height; };
        float raw[2];
    };
} v2; 


typedef struct v3 { 
    union{
        struct { float x,y,z; };
        float raw[3];
    };
}  v3; 


typedef struct v4 { 
    union
    {
        struct { float x,y,z, w; };
        float raw[4];
    };
}v4; 


#define V2(x, y)        (v2){{{x, y}}}
#define V3(x, y, z)     (v3){{{x, y, z}}}
#define V4(x, y, z, w)  (v4){{{x, y, z, w}}}

float GetUnsignedFloat(float x);

int RoundFloat(float num);

#define ArraySize(array, type) (sizeof(array) / sizeof(type))


#define IsBitSet(variable, bit) (variable & (1 << bit))
#define ToggleBit(variable, bit) variable ^= (1 << bit)
#define SetBit(variable, bit) variable |= (1 << bit)
#define ClrBit(variable, bit) variable &= ~(1 << bit)
#endif // __TYPES_H_