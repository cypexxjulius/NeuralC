#include "buffer.h"

#include <stdlib.h>
#include <stdarg.h>
#include "src/core/error.h"
#include "src/platform/memory.h"

extern int GetGLTypeSize(unsigned int type)
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

extern VertexBuffer* NewVertexBuffer(void *data, unsigned int size)
{
    VertexBuffer* this = CreateObject(VertexBuffer);

    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return this;
}

extern void VertexBufferSetLayout(VertexBuffer* this, unsigned int count, ...)
{

    va_list args;
    va_start(args, count);
    
    this->elements = NewVector(2 + count, sizeof(VertexBufferElement*), VECTOR_POINTER | VECTOR_FREE);
    this->stride = 0;
    this->offset = 0;

    for(unsigned int i = 0; i < count; i++)
    {
        
        VertexBufferElement *StoredElement = CreateObject(VertexBufferElement);
        *StoredElement = va_arg(args, VertexBufferElement);
        

        int size = GetGLTypeSize(StoredElement->type) * StoredElement->count; // Size of the Layout

        this->stride += size; 
        
        StoredElement->offset = this->offset;
        this->offset += size;
        
        VectorAdd(this->elements, StoredElement);
    }

    va_end(args);
}


extern void VertexBufferPushLayout
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

extern void DeleteVertexBuffer(VertexBuffer* this)
{
    glDeleteBuffers(1, &this->id);

    DeleteVector(this->elements);
    Memory.Free(this);
}

/*
IndexBuffer
*/

extern IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count)
{
    IndexBuffer* this = CreateObject(IndexBuffer);
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    this->count = count;
    return this;
}

extern void DeleteIndexBuffer(IndexBuffer* this)
{
    glDeleteBuffers(1, &this->id);
    Memory.Free(this);
}

