#include "shader.h"

#include "../utils/fileio.h"
#include "../core/error.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int n_compileShader(char *shaderSrc, n_ShaderType type)
{
    GLenum glType = (type == 2) ? GL_VERTEX_SHADER : (type == 3) ? GL_FRAGMENT_SHADER : GL_FALSE;

    unsigned int id = glCreateShader(glType);

    glShaderSource(id, 1, (const char **)&shaderSrc, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {  
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *message = calloc(length, sizeof(char));
        printf("Shader: %s\n", type == 2 ? "Vertex shader" : "Fragement shader");
        glGetShaderInfoLog(id, length, &length, message);

        ASSERT(0, message);

        free(message);
    }   

    return id;
}

extern void n_createShader(n_window* window, char* vertexShaderPath, char* fragmentShaderPath)
{
    char *vertexShader    = n_readFile(vertexShaderPath);
    char *fragmentShader  = n_readFile(fragmentShaderPath);    

    unsigned int program = glCreateProgram();

    unsigned int vertexShaderID     = n_compileShader(vertexShader, VertexShaderType);
    unsigned int fragmentShaderID  = n_compileShader(fragmentShader, FragmentShaderType);


    free(vertexShader);
    free(fragmentShader);

    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);

    glLinkProgram(program);

    glDetachShader(program, vertexShaderID);
    glDetachShader(program, fragmentShaderID);


    glValidateProgram(program);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    window->shader = program;
}
