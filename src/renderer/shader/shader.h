#ifndef __SHADER_H_
#define __SHADER_H_


#include "../../libinclude/cglm.h"

struct n_Window;

typedef unsigned int n_Shader;


extern n_Shader newShader(char* vertexShaderPath, char* fragmentShaderPath);

extern void shaderBind(n_Shader this);

extern void shaderUnbind();

extern void deleteShader(n_Shader this);

/*
Uniforms 
*/

extern void shaderUploadUniform1m4(n_Shader this, char* name, mat4s matrix);


extern void shaderUploadUniform1f(n_Shader this, char* name, float float0);

extern void shaderUploadUniform1i(n_Shader this, char* name, int number);

#endif // __SHADER_H_