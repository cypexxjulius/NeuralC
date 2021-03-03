#ifndef __SHADER_H_
#define __SHADER_H_


#include <cglm/cglm.h>
#include <cglm/struct.h>

struct n_Window;

extern void newShader(struct n_Window* window, char* vertexShaderPath, char* fragmentShaderPath);

extern void shaderBind(struct n_Window* window);

extern void shaderUnbind();

extern void deleteShader(struct n_Window* window);

/*
Uniforms 
*/

extern void shaderUploadUniform1m4(struct n_Window* window, char* name, mat4s matrix);


extern void shaderUploadUniform1f(struct n_Window* window, char* name, float float0);

#endif // __SHADER_H_