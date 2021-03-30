#ifndef __VERTEX_BUFFER_LAYOUT_H_
#define __VERTEX_BUFFER_LAYOUT_H_ 


#include <stdarg.h>


unsigned int GetOpenGLSizeType(unsigned int type);

struct Vector;

typedef struct VertexBufferElement
{
    unsigned int count, index, type, normalized, typesize;
}VertexBufferElement;

#define BufferElement(count, type) (VertexBufferElement) {count, 0, type, (type == GL_UNSIGNED_BYTE) ? 1 : 0, GetOpenGLSizeType(type)}

typedef struct VertexBufferLayout
{

    struct Vector* elements;
    unsigned int stride;
}VertexBufferLayout;



extern VertexBufferLayout* NewVertexBufferLayout(unsigned int count, ...);

extern void vertexBufferLayoutPush
(VertexBufferLayout* this, unsigned int type, unsigned int count);

extern void DeleteVertexBufferLayout(VertexBufferLayout * this);



#endif // __VERTEX_BUFFER_LAYOUT_H_