#ifndef __SHADER_H_
#define __SHADER_H_

struct n_Window;

extern void newShader(struct n_Window* window, char* vertexShaderPath, char* fragmentShaderPath);

extern void shaderBind(struct n_Window* window);

extern void shaderUnbind();

extern void deleteShader(struct n_Window* window);

/*
Uniforms 
*/

extern void shaderUploadUniform1m4(struct n_Window* window, char* name, float floatarray[4]);


extern void shaderUploadUniform1f(struct n_Window* window, char* name, float float0);

#endif // __SHADER_H_