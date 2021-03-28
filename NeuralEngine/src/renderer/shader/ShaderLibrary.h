#ifndef __SHADERLIBRARY_H_
#define __SHADERLIBRARY_H_

#include "shader.h"
#include "src/utils/types.h"



typedef struct ShaderLibrary 
{   
    Vector* ShaderCollection;
} ShaderLibrary;


ShaderLibrary* newShaderLibrary(unsigned int startCapacity);

Shader* ShaderLibraryLoadShader(ShaderLibrary* this, char* ShaderName, char* vertexShaderPath, char* fragmentShaderPath);

void ShaderLibraryAddShader(ShaderLibrary* this, Shader* shader);

Shader* ShaderLibraryGetShader(ShaderLibrary* this, char* ShaderName);

void deleteShaderLibrary(ShaderLibrary* this);

#endif // __SHADERLIBRARY_H_