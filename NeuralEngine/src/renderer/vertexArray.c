#include "vertexArray.h"
#include "buffer.h"
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

extern void VertexArrayAddVertexBuffer
(VertexArray* this, VertexBuffer* vertexBuffer)
{
    VertexArrayBind(this);
    VertexBufferBind(vertexBuffer);

    this->index = 0;
    for(unsigned int i = 0; i < VectorLength(vertexBuffer->elements); i++)
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