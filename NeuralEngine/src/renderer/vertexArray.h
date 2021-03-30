#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "vertexBufferLayout.h"
#include "buffer.h"


typedef struct VertexArray
{
    unsigned int rendererID;
} VertexArray;


extern VertexArray* NewVertexArray();

extern void vertexArrayAddBuffer
(VertexArray* this, VertexBuffer* vertexBuffer, VertexBufferLayout* layout);

extern void vertexArrayBind(VertexArray *this);

extern void vertexArrayUnbind();

extern void DeleteVertexArray(VertexArray *this);

#endif // __VERTEX_ARRAY_H_