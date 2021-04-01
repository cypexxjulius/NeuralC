#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "vertexBufferLayout.h"
#include "buffer.h"
#include "src/platform/memory.h"

typedef struct VertexArray
{
    unsigned int rendererID;
} VertexArray;


extern VertexArray* NewVertexArray();

extern void vertexArrayAddBuffer
(VertexArray* this, VertexBuffer* vertexBuffer, VertexBufferLayout* layout);

extern inline void DeleteVertexArray(VertexArray* this)
{
    glDeleteVertexArrays(1, &this->rendererID);
    MemFree(this);
}

extern inline void vertexArrayBind(VertexArray* this)
{
    glBindVertexArray(this->rendererID);
}

extern inline void vertexArrayUnbind()
{
    glBindVertexArray(0);
}


#endif // __VERTEX_ARRAY_H_