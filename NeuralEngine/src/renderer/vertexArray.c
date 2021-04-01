#include "vertexArray.h"
#include "buffer.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include <glad/glad.h>

extern VertexArray* NewVertexArray()
{
    VertexArray* this = CreateObject(VertexArray);
    glGenVertexArrays(1, &this->rendererID); 

    return this;
}

extern void vertexArrayAddBuffer
(VertexArray* this, VertexBuffer* vertexBuffer, VertexBufferLayout* layout)
{
    vertexArrayBind(this);
    vertexBufferBind(*vertexBuffer);

    unsigned int offset = 0;
    for(unsigned int i = 0; i < VectorLength(layout->elements); i++)
    {
        VertexBufferElement* element = VectorGet(layout->elements, i);

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element->count, element->type, element->normalized, layout->stride, (const void*)offset);

        offset += element->typesize * element->count;
    }
}