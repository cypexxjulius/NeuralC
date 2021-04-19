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
    char name[50];
} Shader;


extern Shader* NewShader(char* ShaderName, char* ShaderPath);


extern void ShaderBind(Shader* this);


extern inline void DeleteShader(Shader* this);


/*
Uniforms 
*/

extern inline void ShaderSetMat4(Shader* this, char* name, mat4 matrix);

extern inline void ShaderSetFloat(Shader* this, char* name, float float0);

extern inline void ShaderSetInt(Shader* this, char* name, int number);

extern inline void ShaderSetFloat4(Shader* this, char* name, v4 vec4);

extern inline void ShaderSetIntArray(Shader* this, char* name, int* values, u32 count);

#endif // __SHADER_H_