#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>
#include <cglm/struct.h>
#include <stdbool.h>

#include "datatypes/Vector.h"

typedef uint8_t byte;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;



typedef struct v2 { float x,y; }    v2; 
typedef struct v3 { float x,y,z; }  v3; 
typedef struct v4 { float x,y,z,w; }v4; 

#define v2(x, y)       (v2){x, y}
#define v3(x, y, z)    (v3){x, y, z}
#define v4(x, y, z, w) (v4){x, y, z, w}

void mat4print(mat4s mat);
float GetUnsignedFloat(float x);

#endif // __TYPES_H_