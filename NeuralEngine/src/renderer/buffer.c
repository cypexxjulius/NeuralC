#include "buffer.h"

#include <stdlib.h>
#include "src/platform/memory.h"

extern VertexBuffer NewVertexBuffer(void *data, unsigned int size)
{
    VertexBuffer this;
    glGenBuffers(1, &this);
    glBindBuffer(GL_ARRAY_BUFFER, this);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return this;
}

/*
IndexBuffer
*/

extern IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count)
{
    IndexBuffer* this = MemAlloc(1 * sizeof(IndexBuffer));
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    return this;
}

extern void DeleteIndexBuffer(IndexBuffer* this)
{
    glDeleteBuffers(1, &this->id);
    MemFree(this);
}

