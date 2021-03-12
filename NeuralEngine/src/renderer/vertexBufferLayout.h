#ifndef __VERTEX_BUFFER_LAYOUT_H_
#define __VERTEX_BUFFER_LAYOUT_H_ 

struct vector;

typedef struct 
{
    unsigned int count, index, type, normalized, typesize;
}n_VertexBufferElement;

typedef struct 
{

    struct vector* elements;
    unsigned int stride;
}n_VertexBufferLayout;

extern n_VertexBufferLayout* newVertexBufferLayout();

extern void vertexBufferLayoutPush
(n_VertexBufferLayout* this, unsigned int type, unsigned int count);

extern void deleteVertexBufferLayout(n_VertexBufferLayout * this);



#endif // __VERTEX_BUFFER_LAYOUT_H_