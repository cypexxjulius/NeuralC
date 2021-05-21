#include "ShaderLibrary.h"
#include "src/platform/memory.h"
#include "src/core/error.h"

#include <string.h>

ShaderLibrary* NewShaderLibrary(unsigned int startCapacity)
{
    ShaderLibrary* this = CreateObject(ShaderLibrary);

    NewVector(&this->ShaderCollection, startCapacity, sizeof(Shader), VECTOR_POINTER);

    return this;
}


Shader* ShaderLibraryLoadShader(ShaderLibrary* this, String ShaderName,  char* ShaderPath)
{
    Shader* shader = NewShaderFromFile(ShaderName, ShaderPath);
    
    Assert(shader == 0, "Shader Creation Failed");
    VectorAdd(&this->ShaderCollection, shader);

    return shader;
}

void ShaderLibraryAddShader(ShaderLibrary* this, Shader* shader)
{
    if(ShaderLibraryGetShader(this, String(shader->name)) == NULL)
        VectorAdd(&this->ShaderCollection, shader);
}

Shader* ShaderLibraryGetShader(ShaderLibrary* this, String ShaderName)
{ 
    Shader* temp;
    for(u32 i = 0; i < this->ShaderCollection.used; i++)
    {
        temp = VectorGet(&this->ShaderCollection, i);
        
        if(strlen(temp->name) != ShaderName.length)
            continue;

        if(Memory.Compare(ShaderName.string, temp->name, ShaderName.length) == 0)
            return temp;
    }
    return NULL;
}


void DeleteShaderLibrary(ShaderLibrary* this)
{
    Shader* temp;
    for(u32 i = 0; i < this->ShaderCollection.used; i++)
    {
        temp = VectorGet(&this->ShaderCollection, i);
        DeleteShader(temp);
    }

    DeleteVector(&this->ShaderCollection);
    Memory.Free(this);
}

