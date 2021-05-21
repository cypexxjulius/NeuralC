#include "Vector.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

#include <math.h>

void NewVector(Vector *this, unsigned int count, unsigned int type_size, VECTOR_FLAGS flags)
{
    *this = (Vector){ 0 };

    this->capacity = count;
    this->used = 0;

    this->type_size = (flags & VECTOR_POINTER) ? sizeof(void *) : type_size;
    this->flags = flags;

    this->data = Memory.Alloc(type_size * count);
}

void VectorAdd(Vector* this, void *element)
{
    if(this->capacity <= this->used)
    {
        unsigned int newAllocSize = this->capacity * 2;
        this->data = Memory.Realloc(this->data, this->type_size * newAllocSize);
        this->capacity = newAllocSize;
    }

    VectorReplace(this, this->used, element);
    this->used++;
}

void VectorReplace(Vector* this, unsigned int index, void* element)
{
    Assert(index >= this->capacity, "Index out of vector range");

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        temp[index] = element;
        return;
    }

    Memory.Copy(
        (byte*)this->data + this->type_size * index, 
        element, 
        this->type_size
    );
}

void VectorRemove(Vector* this, unsigned int index)
{
    Assert(this->used <= index, "Vector index does not exist");

    this->used--;

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;

        if(this->flags & VECTOR_FREE)
            Memory.Free(temp[index]);

        if(this->flags & VECTOR_NOREFILL)
            return; 

        for(unsigned int i = index; i < (unsigned int)this->used; i++)
            temp[i] = temp[i+1];
        
        return;
    }
    if(this->flags & VECTOR_NOREFILL)
        return; 

    Memory.Copy( 
        (byte*)this->data + index * this->type_size,  
        (byte*)this->data + (index +1) * this->type_size, 
        this->type_size * (this->used - index)
    );
    
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

void VectorClear(Vector *this)
{
    if(this->flags & VECTOR_FREE && this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        for(unsigned int i= 0; i < this->used; i++)
        {
            if(temp[i] != NULL)
                Memory.Free(temp[i]);

            temp[i] = NULL;
        }
    } else
    {
        for(unsigned int i = 0; i < this->used * this->type_size; i++)
            *((byte *)(this->data) + i) = 0;
    }

    this->used = 0;
}

void DeleteVector(Vector *this)
{
    if(this->flags & VECTOR_FREE && this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        for(unsigned int i= 0; i < this->used; i++)
        {
            if(temp[i] != NULL)
                Memory.Free(temp[i]);

            temp[i] = NULL;
        }
    }

    Memory.Free(this->data);
}
