#include "vertexBufferLayout.h"

#include "src/utils/types.h"
#include "src/core/error.h"
#include "src/platform/memory.h"

#include <glad/glad.h>

unsigned int GetOpenGLSizeType(unsigned int type)
{
    switch (type)
    {
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
        default:     Assert(1, "Unknown OpenGL type!");
    }

    return 0;
}

extern VertexBufferLayout* NewVertexBufferLayout(unsigned int count, ...)
{

    va_list args;
    va_start(args, count);
    
    VertexBufferLayout* this =  CreateObject(VertexBufferLayout);
    this->elements = NewVector(2 + count, sizeof(VertexBufferElement*), VECTOR_POINTER | VECTOR_FREE);
    this->stride = 0;

    for(unsigned int i = 0; i < count; i++)
    {
        VertexBufferElement element = va_arg(args, VertexBufferElement);
        element.index = i;


        VertexBufferElement *StoredElement = CreateObject(VertexBufferElement);
        *StoredElement = element;

        VectorAdd(this->elements, StoredElement);
        this->stride += element.typesize * count;
    }

    va_end(args);
    return this;
}

extern void vertexBufferLayoutPush
(VertexBufferLayout* this, VertexBufferElement layout)
{
    VertexBufferElement *StoredElement = CreateObject(VertexBufferElement);
    *StoredElement = layout;

    VectorAdd(this->elements, StoredElement);
    this->stride += layout.typesize * layout.count;
}
extern void DeleteVertexBufferLayout(VertexBufferLayout* this)
{
    DeleteVector(this->elements);
    MemFree(this);
}