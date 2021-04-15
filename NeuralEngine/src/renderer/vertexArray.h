#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "buffer.h"
#include "src/platform/memory.h"

typedef struct VertexArray
{
    unsigned int rendererID, index;
    IndexBuffer* indexBuffer;
    Vector* vertexBuffer;
} VertexArray;


extern VertexArray* NewVertexArray();

extern void VertexArrayAddVertexBuffer
(VertexArray* this, VertexBuffer* vertexBuffer);



static inline void DeleteVertexArray(VertexArray* this)
{
    glDeleteVertexArrays(1, &this->rendererID);

    VertexBuffer* vb;
    for(unsigned int i = 0; i < VectorLength(this->vertexBuffer); i++)
    {
        vb = VectorGet(this->vertexBuffer, i);
        DeleteVertexBuffer(vb);
    }

    DeleteVector(this->vertexBuffer);
    DeleteIndexBuffer(this->indexBuffer);
    Memory.Free(this);
}

static inline void VertexArrayBind(VertexArray* this)
{
    glBindVertexArray(this->rendererID);
}

static inline void VertexArraySetIndexBuffer(VertexArray* this, IndexBuffer* indexBuffer)
{
    VertexArrayBind(this);
    IndexBufferBind(indexBuffer);

    this->indexBuffer = indexBuffer;
}

static inline void VertexArrayUnbind()
{
    glBindVertexArray(0);
}


#endif // __VERTEX_ARRAY_H_