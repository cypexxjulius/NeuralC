#ifndef __BUFFER_H_
#define __BUFFER_H_


#include <glad/glad.h>

/*
VertexBuffer
*/
typedef unsigned int VertexBuffer;

extern VertexBuffer NewVertexBuffer(void *data, unsigned int size); 


extern inline void vertexBufferBind(VertexBuffer this)
{
    glBindBuffer(GL_ARRAY_BUFFER, this);
}

extern inline void vertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


extern inline void DeleteVertexBuffer(VertexBuffer this)
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

extern inline void indexBufferBind(IndexBuffer* this)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

extern inline void indexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

extern void DeleteIndexBuffer(IndexBuffer* this);


#endif // __BUFFER_H_