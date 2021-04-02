#ifndef __BUFFER_H_
#define __BUFFER_H_


#include <glad/glad.h>

/*
VertexBuffer
*/
typedef unsigned int VertexBuffer;

extern VertexBuffer NewVertexBuffer(void *data, unsigned int size); 


static inline void vertexBufferBind(VertexBuffer this)
{
    glBindBuffer(GL_ARRAY_BUFFER, this);
}

static inline void vertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


static inline void DeleteVertexBuffer(VertexBuffer this)
{
    glDeleteBuffers(1, &this);
}

/*
IndexBuffer
*/

typedef struct 
{
    unsigned int id;
    unsigned int count;
}IndexBuffer;


extern IndexBuffer* NewIndexBuffer(unsigned int *data, unsigned int count); 

static inline void indexBufferBind(IndexBuffer* this)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

static inline void indexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

extern void DeleteIndexBuffer(IndexBuffer* this);


#endif // __BUFFER_H_