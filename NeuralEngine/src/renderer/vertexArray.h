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



extern inline void DeleteVertexArray(VertexArray* this);

extern inline void VertexArrayBind(VertexArray* this);

extern inline void VertexArraySetIndexBuffer(VertexArray* this, IndexBuffer* indexBuffer);

extern inline VertexBuffer* VertexArrayGetVertexBuffer(VertexArray* this, u32 index);

extern inline void VertexArrayUnbind();

#endif // __VERTEX_ARRAY_H_