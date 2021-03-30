#ifndef __SHADERLIBRARY_H_
#define __SHADERLIBRARY_H_

#include "shader.h"
#include "src/utils/types.h"



typedef struct ShaderLibrary 
{   
    Vector* ShaderCollection;
} ShaderLibrary;


ShaderLibrary* NewShaderLibrary(unsigned int startCapacity);

Shader* ShaderLibraryLoadShader(ShaderLibrary* this, char* ShaderName,char* ShaderPath);

void ShaderLibraryAddShader(ShaderLibrary* this, Shader* shader);

Shader* ShaderLibraryGetShader(ShaderLibrary* this, char* ShaderName);

void DeleteShaderLibrary(ShaderLibrary* this);

#endif // __SHADERLIBRARY_H_