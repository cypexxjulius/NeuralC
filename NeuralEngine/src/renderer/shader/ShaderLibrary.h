#ifndef __SHADER_LIBRARY_H_
#define __SHADER_LIBRARY_H_

#include "shader.h"
#include "src/utils/types.h"



typedef struct ShaderLibrary 
{   
    Vector* ShaderCollection;
} ShaderLibrary;


ShaderLibrary* NewShaderLibrary(unsigned int startCapacity);

Shader* ShaderLibraryLoadShader(ShaderLibrary* this, String ShaderName,char* ShaderPath);

void ShaderLibraryAddShader(ShaderLibrary* this, Shader* shader);

Shader* ShaderLibraryGetShader(ShaderLibrary* this, String ShaderName);

void DeleteShaderLibrary(ShaderLibrary* this);

#endif // __SHADER_LIBRARY_H_