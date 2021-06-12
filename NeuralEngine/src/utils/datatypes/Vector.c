#include "Vector.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

void NewVector(Vector *this, unsigned int count, unsigned int type_size, VECTOR_FLAGS flags)
{
    *this = (Vector){ 0 };

    this->capacity = count;
    this->used = 0;

    this->type_size = (flags & VECTOR_POINTER) ? sizeof(void *) : type_size;
    this->flags = flags;

    this->data = Memory.Alloc(type_size * count);
}

void VectorResize(Vector* this, unsigned int newSize)
{
    this->data = Memory.Realloc(this->data, this->type_size * newSize);
    this->capacity = newSize;
}

void VectorAdd(Vector* this, void *element)
{
    if(this->capacity <= this->used)
        VectorResize(this, this->capacity * 2);

    VectorReplace(this, this->used++, element);
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

    
    byte* valuePtr = (byte*)this->data + index * this->type_size;
    Memory.Copy(valuePtr, element, this->type_size);
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

        for(unsigned int i = index; i < (unsigned int)this->used - 1; i++)
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
        return ((byte **)(this->data))[index];

    return (byte*)this->data + index * this->type_size;
}

void VectorClear(Vector *this)
{
    if(this->flags & VECTOR_FREE && this->flags & VECTOR_POINTER)
    {
        for(unsigned int i= 0; i < this->used; i++)
            Memory.Free(VectorGet(this, i));

    } 
    else
        memset(this->data, 0, this->used * this->type_size);

    this->used = 0;
}

void DeleteVector(Vector *this)
{
    if(this->flags & VECTOR_FREE && this->flags & VECTOR_POINTER)
    {
        for(unsigned int i= 0; i < this->used; i++)
            Memory.Free(VectorGet(this, i));
    }

    Memory.Free(this->data);
}
