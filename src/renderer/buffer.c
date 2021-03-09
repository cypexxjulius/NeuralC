#include "buffer.h"

#include <stdlib.h>
#include "../libinclude/glad.h"
#include "../platform/memory.h"

extern n_VertexBuffer newVertexBuffer(void *data, unsigned int size)
{
    n_VertexBuffer this;
    glGenBuffers(1, &this);
    glBindBuffer(GL_ARRAY_BUFFER, this);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return this;
}

extern void vertexBufferBind(n_VertexBuffer this)
{
    glBindBuffer(GL_ARRAY_BUFFER, this);
}

extern void vertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

extern void deleteVertexBuffer(n_VertexBuffer this)
{
    glDeleteBuffers(1, &this);
}

/*
IndexBuffer
*/

extern n_IndexBuffer* newIndexBuffer(unsigned int *data, unsigned int count)
{
    n_IndexBuffer* this = nl_malloc(1 * sizeof(n_IndexBuffer));
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    return this;
}

extern void indexBufferBind(n_IndexBuffer* this)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

extern void indexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

extern void deleteIndexBuffer(n_IndexBuffer* this)
{
    glDeleteBuffers(1, &this->id);
    nl_free(this);
}

