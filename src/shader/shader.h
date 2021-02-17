#ifndef __SHADER_H_
#define __SHADER_H_

#include "../core/window.h"

typedef enum ShaderType 
{
    VertexShaderType = 2,
    FragmentShaderType = 3
} n_ShaderType;


extern void n_createShader(n_Window* window, char* vertexShaderPath, char* fragmentShaderPath);


#endif // __SHADER_H_