#include "Vector.h"

#include "src/platform/memory.h"
Vector *newVector(unsigned int count, unsigned int type_size, VECTOR_FLAGS flags)
{
    Vector* this = MemAlloc(sizeof(Vector));

    this->capacity = count;
    this->used = 0;

    this->type_size = (flags & VECTOR_POINTER) ? sizeof(void *) : type_size;
    this->flags = flags;

    this->data = MemAlloc(type_size * count);

    return this;
}

void VectorAdd(Vector* this, void *element)
{
    if(this->capacity == this->used)
    {
        this->data = MemRealloc(this->data, this->type_size * (this->capacity + 1));
        this->capacity++;
    }

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        temp[this->used] = element;
    }
    else 
    {
        memcpy((byte*)this->data + this->type_size * this->used, element, this->type_size);
    }

    this->used++;
}
void VectorRemove(Vector* this, unsigned int indices)
{
    if(this->used <= indices)
        return; 

    if(this->flags & VECTOR_FREE)
    {
        void **temp = this->data;
        MemFree(temp[indices]);
    }
    memcpy((byte*)this->data + indices * this->type_size, (byte*)this->data + (indices +1) * this->type_size, this->type_size * (this->used - indices - 1));
    this->used--;
}

void* VectorGet(Vector* this, unsigned int indices)
{
    if(this->used <= indices)
        return NULL; 
    
    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        return temp[indices];
    }

    return (byte*)this->data + indices * this->type_size;
}

void deleteVector(Vector *this)
{
    if(this->flags & VECTOR_FREE)
    {
        void **temp = this->data;
        for(unsigned int i= 0; i < this->used; i++)
            free(temp[i]);
    }
    MemFree(this->data);

    MemFree(this);
}

unsigned int VectorLength(Vector *this)
{
    return this->used;
}
