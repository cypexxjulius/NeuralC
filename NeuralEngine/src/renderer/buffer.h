#ifndef __BUFFER_H_
#define __BUFFER_H_

#include "src/utils/types.h"
#include <glad/glad.h>

enum NEURAL_TYPES
{
    NEURAL_NO_TYPE,
    NEURAL_FLOAT = GL_FLOAT,
    NEURAL_INT = GL_INT,
    NEURAL_CHAR = GL_BYTE,
    NEURAL_BOOL = GL_BOOL,
};

typedef struct VertexBufferElement
{
    unsigned int count, offset, normalized, type;
} VertexBufferElement;

#define BufferElement(itype, icount) (VertexBufferElement) { .count=icount, .offset=0, .normalized=(itype == GL_BYTE) ? 1 : 0, .type=itype}

extern int GetGLTypeSize(unsigned int type);

/*
VertexBuffer
*/
typedef struct VertexBuffer {
    unsigned int id, stride, offset;
    Vector* elements;
} VertexBuffer;

extern VertexBuffer* NewVertexBuffer(void *data, unsigned int size); 


static inline void VertexBufferBind(VertexBuffer* this)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

static inline void VertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

extern void DeleteVertexBuffer(VertexBuffer* this);


extern void VertexBufferPushLayout
(VertexBuffer* this, VertexBufferElement layout);


extern void VertexBufferSetLayout(VertexBuffer* this, unsigned int count, ...);

/*
IndexBuffer
*/

typedef struct 
{
    unsigned int id;
    unsigned int count;
}IndexBuffer;


extern IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count); 

static inline void IndexBufferBind(IndexBuffer* this)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

static inline void IndexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

extern void DeleteIndexBuffer(IndexBuffer* this);


#endif // __BUFFER_H_