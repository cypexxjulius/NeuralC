#include "vertexArray.h"
#include "Buffer.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include <glad/glad.h>



extern VertexArray* NewVertexArray()
{
    VertexArray* this = CreateObject(VertexArray);
    this->index = 0;
    glGenVertexArrays(1, &this->rendererID); 
    this->vertexBuffer = NewVector(2, sizeof(VertexBuffer), VECTOR_POINTER);
    return this;
}

void VertexArrayAddVertexBuffer
(VertexArray* this, VertexBuffer* vertexBuffer)
{
    VertexArrayBind(this);
    VertexBufferBind(vertexBuffer);

    this->index = 0;
    for(unsigned int i = 0; i < vertexBuffer->elements->used; i++)
    {
        VertexBufferElement* element = VectorGet(vertexBuffer->elements, i);

        
        glEnableVertexAttribArray(this->index);
        glVertexAttribPointer(this->index, 
                            element->count, 
                            element->type, 
                            element->normalized ? GL_TRUE : GL_FALSE, 
                            vertexBuffer->stride, 
                            (const void*)(intptr_t)element->offset);
        
        this->index++;
    }
    VectorAdd(this->vertexBuffer, vertexBuffer);
}

void DeleteVertexArray(VertexArray* this)
{
    glDeleteVertexArrays(1, &this->rendererID);

    VertexBuffer* vb;
    for(unsigned int i = 0; i < this->vertexBuffer->used; i++)
    {
        vb = VectorGet(this->vertexBuffer, i);
        DeleteVertexBuffer(vb);
    }

    DeleteVector(this->vertexBuffer);
    DeleteIndexBuffer(this->indexBuffer);
    Memory.Free(this);
}

void VertexArraySetIndexBuffer(VertexArray* this, IndexBuffer* indexBuffer)
{
    VertexArrayBind(this);
    IndexBufferBind(indexBuffer);

    this->indexBuffer = indexBuffer;
}

void VertexArrayBind(VertexArray* this)
{
    glBindVertexArray(this->rendererID);
}

VertexBuffer* VertexArrayGetVertexBuffer(VertexArray* this, u32 index)
{
    return VectorGet(this->vertexBuffer, index);
}

void VertexArrayUnbind()
{
    glBindVertexArray(0);
}
