#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "src/utils/types.h"

typedef enum VECTOR_FLAGS
{
    VECTOR_POINTER = 4, 
    VECTOR_FREE = 8,
    VECTOR_NOREFILL = 16
} VECTOR_FLAGS;

typedef struct Vector
{  
    unsigned int capacity, type_size, used;
    char flags;
    void *data;
} Vector;


/*
Returns a New Vector with the usable size of count,
can only store types of size type_size,
possible Flags are VECTOR_POINTER and VECTOR_FREE.
VECTOR_POINTER sets the type_size to 8, only pointers can be stored now.
VECTOR_FREE frees the pointers on the Vector's destruction.
Pass in 0 if none of these flags should be set.
*/
void NewVector(Vector* this, unsigned int count, unsigned int type_size, VECTOR_FLAGS flags);

/*
Stores a New element in the Vector,
if there is not enough space to store the element
the space will be resized automatically.
*/
void VectorAdd(Vector* this, void *element);

/*
Removes the element in the array at position indices (starting at 0).
*/
void VectorRemove(Vector* this, unsigned int index);


/*
Returns a null pointer to the indices element if no flag is set,
if the VECTOR_POINTER flag is set the pointer returned is the pointer stored. 
*/
void* VectorGet(Vector* this, unsigned int index);

/*
Free's the Vector, if the VECTOR_FREE flag is set the pointer stored will be freed.
*/
void DeleteVector(Vector *this);

/*
Replaces the element at the index with the new one
*/
void VectorReplace(Vector* this, unsigned int index, void* element);

/*
Resizes the vector to the specified size
*/
void VectorResize(Vector* this, unsigned int newSize);

/*
Clears all elements of the vector, 
if pointers where stored and the VECTOR_FREE flag was enabled,
 they also will be freed
*/
void VectorClear(Vector *this);

#endif // __VECTOR_H_