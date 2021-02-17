#ifndef __BUFFER_H_
#define __BUFFER_H_

/*
VertexBuffer
*/
typedef unsigned int n_VertexBuffer;

extern n_VertexBuffer newVertexBuffer(void *data, unsigned int size); 

extern void vertexBufferBind(n_VertexBuffer this);

extern void vertexBufferUnbind();

extern void deleteVertexBuffer(n_VertexBuffer this);

/*
IndexBuffer
*/

typedef struct 
{
    unsigned int id;
    unsigned int count;
}n_IndexBuffer;


extern n_IndexBuffer* newIndexBuffer(unsigned int *data, unsigned int count); 

extern void indexBufferBind(n_IndexBuffer* this);

extern void indexBufferUnbind();

extern void deleteIndexBuffer(n_IndexBuffer* this);


#endif // __BUFFER_H_