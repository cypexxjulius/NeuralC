#include "vertexArray.h"
#include "buffer.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include <glad/glad.h>

extern n_VertexArray* newVertexArray()
{
    n_VertexArray* this = MemAlloc(1 * sizeof(n_VertexArray));
    glGenVertexArrays(1, &this->rendererID); 

    return this;
}

extern void deleteVertexArray(n_VertexArray* this)
{
    glDeleteVertexArrays(1, &this->rendererID);
    MemFree(this);
}

extern void vertexArrayBind(n_VertexArray* this)
{
    glBindVertexArray(this->rendererID);
}

extern void vertexArrayUnbind()
{
    glBindVertexArray(0);
}


extern void vertexArrayAddBuffer
(n_VertexArray* this, n_VertexBuffer* vertexBuffer, n_VertexBufferLayout* layout)
{
    vertexArrayBind(this);
    vertexBufferBind(* vertexBuffer);
    unsigned int offset = 0;
    for(unsigned int i = 0; i < layout->elements->used; i++)
    {
        n_VertexBufferElement* element = VectorGet(layout->elements, i);

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element->count, element->type, element->normalized, layout->stride, (const void*)offset);

        offset += element->typesize * element->count;
    }
}