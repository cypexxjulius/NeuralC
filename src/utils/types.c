#include "types.h"
#include <stdlib.h>
#include <string.h>

vector *newVector(unsigned int count, unsigned int type_size, VECTOR_FLAGS flags)
{
    vector* this = malloc(sizeof(vector));

    this->capacity = count;
    this->used = 0;

    this->type_size = (flags & VECTOR_POINTER) ? sizeof(void *) : type_size;
    this->flags = flags;

    this->data = malloc(type_size * count);

    return this;
}

void vectorAdd(vector* this, void *element)
{
    if(this->capacity == this->used)
    {
        this->data = realloc(this->data, this->type_size * (this->capacity + 1));
        this->capacity++;
    }

    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        temp[this->used] = element;
    }
    else 
    {
        memcpy(this->data + this->type_size * this->used, element, this->type_size);
    }

    this->used++;
}
void vectorRemove(vector* this, unsigned int indices)
{
    if(this->used <= indices)
        return; 

    if(this->flags & VECTOR_FREE)
    {
        void **temp = this->data;
        free(temp[indices]);
    }
    memcpy(this->data + indices * this->type_size, this->data + (indices +1) * this->type_size, this->type_size * (this->used - indices - 1));
    this->used--;
}

void* vectorGet(vector* this, unsigned int indices)
{
    if(this->used <= indices)
        return NULL; 
    
    if(this->flags & VECTOR_POINTER)
    {
        void **temp = this->data;
        return temp[indices];
    }

    return this->data + indices * this->type_size;
}

void deleteVector(vector *this)
{
    if(this->flags & VECTOR_FREE)
    {
        void **temp = this->data;
        for(int i= 0; i < this->used; i++)
            free(temp[i]);
    }
    free(this->data);

    free(this);
}

unsigned int vectorLength(vector *this)
{
    return this->used;
}