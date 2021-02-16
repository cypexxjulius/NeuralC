#ifndef __TYPES_H_
#define __TYPES_H_

typedef enum vector_type
{
    VECTOR_POINTER = 4, VECTOR_FREE = 8
} VECTOR_FLAGS;

typedef struct vector
{  
    void *data;
    unsigned int capacity;
    unsigned int type_size;
    unsigned int used;
    char flags;
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

#endif // __TYPES_H_