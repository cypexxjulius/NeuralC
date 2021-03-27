#include "vertexBufferLayout.h"

#include "src/utils/types.h"
#include "src/core/error.h"
#include "src/platform/memory.h"

#include <glad/glad.h>

static unsigned int convertToSize(GLenum type)
{
    switch (type)
    {
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
    }
    ASSERT(0, "Unknown type!");

    return 0;
}

extern n_VertexBufferLayout* newVertexBufferLayout()
{
    n_VertexBufferLayout* this =  MemCalloc(1, sizeof(n_VertexBufferLayout));
    this->elements = newVector(2, sizeof(n_VertexBufferElement), 0);
    this->stride = 0;
    return this;
}

extern void vertexBufferLayoutPush
(n_VertexBufferLayout* this, unsigned int type, unsigned int count)
{
    n_VertexBufferElement element;
    element.type = type;
    element.count = count;
    element.normalized = (type == GL_UNSIGNED_BYTE) ? 1 : 0;
    element.typesize = convertToSize(type);

    vectorAdd(this->elements, &element);
    this->stride += convertToSize(type) * count;
}
extern void deleteVertexBufferLayout(n_VertexBufferLayout* this)
{
    deleteVector(this->elements);
    MemFree(this);
}