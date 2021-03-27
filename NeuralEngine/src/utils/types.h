#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>
#include <cglm/struct.h>
#include <stdbool.h>

typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t byte;
typedef uint8_t u8;



typedef struct v2 { float x,y; }v2; 
typedef struct v3 { float x,y,z; }v3; 
typedef struct v4 { float x,y,z,w; }v4; 

#define v2(x, y)       (v2){x, y}
#define v3(x, y, z)    (v3){x, y, z}
#define v4(x, y, z, w) (v4){x, y, z, w}

void mat4print(mat4s mat);
float GetUnsignedFloat(float x);

typedef enum VECTOR_FLAGS
{
    VECTOR_POINTER = 4, VECTOR_FREE = 8
} VECTOR_FLAGS;

typedef struct vector
{  
    u32 capacity;
    u32 type_size;
    u32 used;
    char flags;
    void *data;
} vector;


/*
Returns a new vector with the usable size of count,
can only store types of size type_size,
possible Flags are VECTOR_POINTER and VECTOR_FREE.
VECTOR_POINTER sets the type_size to 8, only pointers can be stored now.
VECTOR_FREE frees the pointers on the vector's destruction.
Pass in 0 if none of these flags should be set.
*/
vector *newVector(unsigned int count, unsigned int type_size, VECTOR_FLAGS flags);

/*
Stores a new element in the vector,
if there is not enough space to store the element
the space will be resized automatically.
*/
void vectorAdd(vector* this, void *element);

/*
Removes the element in the array at position indices (starting at 0).
*/
void vectorRemove(vector* this, unsigned int indices);


/*
Returns a null pointer to the indices element if no flag is set,
if the VECTOR_POINTER flag is set the pointer returned is the pointer stored. 
*/
void* vectorGet(vector* this, unsigned int indices);

/*
Free's the vector, if the VECTOR_FREE flag is set the pointer stored will be freed.
*/
void deleteVector(vector *this);


/*
Returns the length of the vector.
*/
unsigned int vectorLength(vector *this);

/*

TODO implement hash Table

Basic hash-table implementation


typedef struct HashTable
{
    unsigned int size;
    int* table
}HashTable;

HashTable* newHashTable(unsigned int size);

extern void hashTableStore
(HashTable* this, void *accessData, unsigned int accessDataLength, unsigned int data);

extern unsigned int hashTableGet
(HashTable* this, void *accessData, unsigned int accessDataLength);

extern void deleteHashTable(HashTable* this);

*/


#endif // __TYPES_H_