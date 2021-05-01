#include "Buffer.h"

#include <stdlib.h>
#include <stdarg.h>
#include <glad/glad.h>
#include "src/core/error.h"
#include "src/platform/memory.h"

int GetGLTypeSize(unsigned int type)
{
    switch(type)
    {
        case NEURAL_FLOAT:  return sizeof(GLfloat);
        case NEURAL_INT:    return sizeof(GLint);
        case NEURAL_BOOL:   return sizeof(GLboolean);
        case NEURAL_CHAR:   return sizeof(GLbyte);
    }
    Assert(1, "Invalid type");
    return 0;
}

VertexBuffer* NewVertexBuffer(void *data, unsigned int size)
{
    VertexBuffer* this = CreateObject(VertexBuffer);

    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return this;
}

VertexBuffer* NewVertexBufferEmpty(unsigned int size)
{
    VertexBuffer* this = CreateObject(VertexBuffer);

    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

    return this;
}

void VertexBufferSetLayout(VertexBuffer* this, unsigned int count, VertexBufferElement *array)
{ 
    this->elements = NewVector(2 + count, sizeof(VertexBufferElement*), VECTOR_POINTER | VECTOR_FREE);
    this->stride = 0;
    this->offset = 0;

    for(unsigned int i = 0; i < count; i++)
    {
        
        VertexBufferElement *StoredElement = CreateObject(VertexBufferElement);
        *StoredElement = array[i];
        

        int size = GetGLTypeSize(StoredElement->type) * StoredElement->count; // Size of the Layout

        this->stride += size; 
        
        StoredElement->offset = this->offset;
        this->offset += size;
        
        VectorAdd(this->elements, StoredElement);
    }
}


void VertexBufferPushLayout
(VertexBuffer* this, VertexBufferElement layout)
{
    unsigned int size = 0;
    VertexBufferElement *StoredElement = CreateObject(VertexBufferElement);
    *StoredElement = layout;

    size = GetGLTypeSize(layout.type) * layout.count;
    StoredElement->offset = this->offset;

    this->offset += size;
    this->stride += size;
    VectorAdd(this->elements, StoredElement);
    
}

inline void DeleteVertexBuffer(VertexBuffer* this)
{
    glDeleteBuffers(1, &this->id);

    DeleteVector(this->elements);
    Memory.Free(this);
}


inline void VertexBufferBind(VertexBuffer* this)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

inline void VertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void VertexBufferSetData(VertexBuffer* this, const void* data, u32 size)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}


/*
IndexBuffer
*/

IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count)
{
    IndexBuffer* this = CreateObject(IndexBuffer);
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    this->count = count;
    return this;
}


extern inline void IndexBufferBind(IndexBuffer* this)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void inline IndexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void inline DeleteIndexBuffer(IndexBuffer* this)
{
    glDeleteBuffers(1, &this->id);
    Memory.Free(this);
}

