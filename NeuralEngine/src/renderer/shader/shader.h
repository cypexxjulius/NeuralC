#ifndef __SHADER_H_
#define __SHADER_H_

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>
#include "src/utils/types.h"

struct Window;

typedef struct Shader
{
    unsigned int ShaderID;
    u64 flags;
    char name[50];
} Shader;

typedef enum SHADER_FLAGS
{
    SHADER_FLAGS_imageArraySet = (1 << 1),
} SHADER_FLAGS;


Shader *NewShaderFromString(const String ShaderName, const char* fragmentShader, const char* vertexShader);

Shader* NewShaderFromFile(const String ShaderName, char* ShaderPath);

void ShaderBind(Shader* this);

void DeleteShader(Shader* this);


/*
Uniforms 
*/

void ShaderSetMat4(Shader* this, char* name, mat4 matrix);

void ShaderSetFloat(Shader* this, char* name, float float0);

void ShaderSetInt(Shader* this, char* name, int number);

void ShaderSetFloat4(Shader* this, char* name, v4 vec4);

void ShaderSetIntArray(Shader* this, char* name, int* values, u32 count);

#endif // __SHADER_H_