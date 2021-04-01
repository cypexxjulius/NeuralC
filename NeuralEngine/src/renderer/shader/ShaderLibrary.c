#include "ShaderLibrary.h"
#include "src/platform/memory.h"
#include "src/core/error.h"

ShaderLibrary* NewShaderLibrary(unsigned int startCapacity)
{
    ShaderLibrary* this = CreateObject(ShaderLibrary);

    this->ShaderCollection = NewVector(startCapacity, sizeof(Shader), VECTOR_POINTER);

    return this;
}


Shader* ShaderLibraryLoadShader(ShaderLibrary* this, char* ShaderName,  char* ShaderPath)
{
    Shader* shader = NewShader(ShaderName, ShaderPath);
    
    
    
    Assert(!shader, "Shader Creation Failed");
        

    VectorAdd(this->ShaderCollection, shader);

    return shader;
}

void ShaderLibraryAddShader(ShaderLibrary* this, Shader* shader)
{
    Assert(ShaderLibraryGetShader(this, shader->name), "Shader already exists in ShaderLibrary");
    VectorAdd(this->ShaderCollection, shader);
}

Shader* ShaderLibraryGetShader(ShaderLibrary* this, char* ShaderName)
{ 
    Shader* temp;
    for(u32 i = 0; i < this->ShaderCollection->used; i++)
    {
        temp = VectorGet(this->ShaderCollection, i);
        if(strcmp(ShaderName, temp->name) == 0)
            return temp;
    }
    return NULL;
}


void DeleteShaderLibrary(ShaderLibrary* this)
{
    Shader* temp;
    for(u32 i = 0; i < this->ShaderCollection->used; i++)
    {
        temp = VectorGet(this->ShaderCollection, i);
        DeleteShader(temp);
    }
}

