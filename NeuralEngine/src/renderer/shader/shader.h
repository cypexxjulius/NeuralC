#ifndef __SHADER_H_
#define __SHADER_H_


#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>

struct Window;

typedef struct Shader
{
    unsigned int ShaderID;
    char name[50];
} Shader;


extern Shader* newShader(char* ShaderName, char* vertexShaderPath, char* fragmentShaderPath);

extern void shaderBind(Shader* this);

extern void shaderUnbind();

extern void deleteShader(Shader* this);

/*
Uniforms 
*/

extern void shaderUploadUniform1m4(Shader* this, char* name, mat4 matrix);


extern void shaderUploadUniform1f(Shader* this, char* name, float float0);

extern void shaderUploadUniform1i(Shader* this, char* name, int number);

#endif // __SHADER_H_