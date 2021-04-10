#include "Vector.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

#include <math.h>

Vector *NewVector(unsigned int count, unsigned int type_size, VECTOR_FLAGS flags)
{
    Vector* this = CreateObject(Vector);
    this->capacity = count;
    this->used = 0;

    this->type_size = (flags & VECTOR_POINTER) ? sizeof(void *) : type_size;
    this->flags = flags;

    this->data = MemAlloc(type_size * count);

    return this;
}

void VectorAdd(Vector* this, void *element)
{
    if(this->capacity <= this->used)
    {
        unsigned int newAllocSize = this->capacity + (int)roundf(this->capacity / 2);
        this->data = MemRealloc(this->data, this->type_size * newAllocSize);
        this->capacity = newAllocSize;
    }

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        temp[this->used] = element;
        this->used++;
        return;
    }

    MemCpy((byte*)this->data + this->type_size * this->used, element, this->type_size);
    this->used++;
}
void VectorRemove(Vector* this, unsigned int index)
{
    Assert(this->used <= index, "Vector index does not exist");

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;

        if(this->flags & VECTOR_FREE)
            MemFree(temp[index]);

        for(unsigned int i = index; i < VectorLength(this) - 1; i++)
        {
            temp[i] = temp[i+1];
        }

        this->used--;
        return;
    }
    MemCpy( 
            (byte*)this->data + index * this->type_size,  
            (byte*)this->data + (index +1) * this->type_size, 
            this->type_size * (this->used - index - 1)
    );


    this->used--;
}

void* VectorGet(Vector* this, unsigned int index)
{
    Assert(this->used <= index, "Invalid vector index accessed");

    
    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        return temp[index];
    }

    return (byte*)this->data + index * this->type_size;
}

void DeleteVector(Vector *this)
{
    if(this->flags & VECTOR_FREE)
    {
        void **temp = this->data;
        for(unsigned int i= 0; i < this->used; i++)
        {
            MemFree(temp[i]);
        }
    }
    MemFree(this->data);

    MemFree(this);
}
