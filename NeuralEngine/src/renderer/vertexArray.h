#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "Buffer.h"

typedef struct VertexArray
{
    unsigned int rendererID, index;
    IndexBuffer* indexBuffer;
    Vector* vertexBuffer;
} VertexArray;


extern VertexArray* NewVertexArray();

extern void VertexArrayAddVertexBuffer
(VertexArray* this, VertexBuffer* vertexBuffer);



void DeleteVertexArray(VertexArray* this);

void VertexArrayBind(VertexArray* this);

void VertexArraySetIndexBuffer(VertexArray* this, IndexBuffer* indexBuffer);

VertexBuffer* VertexArrayGetVertexBuffer(VertexArray* this, u32 index);

void VertexArrayUnbind();

#endif // __VERTEX_ARRAY_H_