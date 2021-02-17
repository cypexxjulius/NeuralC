#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "vertexBufferLayout.h"
#include "buffer.h"


typedef struct n_VertexArray
{
    unsigned int rendererID;
} n_VertexArray;


extern n_VertexArray* newVertexArray();

extern void vertexArrayAddBuffer
(n_VertexArray* this, n_VertexBuffer* vertexBuffer, n_VertexBufferLayout* layout);

extern void vertexArrayBind(n_VertexArray *this);

extern void vertexArrayUnbind();

extern void deleteVertexArray(n_VertexArray *this);

#endif // __VERTEX_ARRAY_H_