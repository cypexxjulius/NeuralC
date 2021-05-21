#ifndef __BUFFER_H_
#define __BUFFER_H_

#include "src/utils/types.h"

enum NEURAL_TYPES
{
    NEURAL_NO_TYPE,
    NEURAL_FLOAT = 0x1406, // GL_FLOAT
    NEURAL_INT = 0x1404,   // GL_INT
    NEURAL_BYTE = 0x1400,  // GL_BYTE
    NEURAL_CHAR = NEURAL_BYTE,  
    NEURAL_BOOL = 0x8b56,  // GL_BOOL
};

typedef struct VertexBufferElement
{
    unsigned int count, offset, normalized, type;
} VertexBufferElement;

#define BufferElement(name, itype, icount) (VertexBufferElement) { .count=icount, .offset=0, .normalized=(itype == NEURAL_BYTE) ? 1 : 0, .type=itype}

extern int GetGLTypeSize(unsigned int type);

/*
VertexBuffer
*/
typedef struct VertexBuffer {
    unsigned int id, stride, offset;
    Vector elements;
} VertexBuffer;

extern VertexBuffer* NewVertexBuffer(void *data, unsigned int size); 

extern VertexBuffer* NewVertexBufferEmpty(unsigned int size);

void VertexBufferBind(VertexBuffer* this);

void VertexBufferUnbind();

void VertexBufferSetData(VertexBuffer* this, const void* data, u32 size);

void DeleteVertexBuffer(VertexBuffer* this);


extern void VertexBufferPushLayout
(VertexBuffer* this, VertexBufferElement layout);


extern void VertexBufferSetLayout(VertexBuffer* this, unsigned int count, VertexBufferElement *array);

/*
IndexBuffer
*/

typedef struct 
{
    unsigned int id;
    unsigned int count;
}IndexBuffer;


extern IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count); 

void IndexBufferBind(IndexBuffer* this);

void IndexBufferUnbind();

void DeleteIndexBuffer(IndexBuffer* this);


#endif // __BUFFER_H_