#include "shader.h"

#include "../utils/fileio.h"
#include "../core/error.h"
#include "../core/window.h"

#include <stdlib.h>
#include <stdio.h>

typedef enum ShaderType 
{
    VertexShaderType = 2,
    FragmentShaderType = 3
} n_ShaderType;


static unsigned int compileShader(char *shaderSrc, n_ShaderType type)
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
        printf("Shader: %s\n", (type == VertexShaderType) ? "Vertex shader" : "Fragement shader");
        glGetShaderInfoLog(id, length, &length, message);

        ASSERT(0, message);

        free(message);
    }   

    return id;
}

extern void newShader(struct n_Window* window, char* vertexShaderPath, char* fragmentShaderPath)
{
    char *vertexShader    = n_readFile(vertexShaderPath);
    char *fragmentShader  = n_readFile(fragmentShaderPath);    

    unsigned int program = glCreateProgram();

    unsigned int vertexShaderID     = compileShader(vertexShader, VertexShaderType);
    unsigned int fragmentShaderID   = compileShader(fragmentShader, FragmentShaderType);


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

extern void shaderBind(struct n_Window* window)
{   
    if(!window->shader)
    {
        ASSERT(0, "No shader created before binding");
        return;
    }

    glUseProgram(window->shader);
}   

extern void shaderUnbind()
{
    glUseProgram(0);
}

extern void deleteShader(struct n_Window* window)
{
    glDeleteProgram(window->shader);
}

static inline int getUniform(n_Window* window, char *name)
{

    unsigned int location = glGetUniformLocation(window->shader, name);
    ASSERT(location != -1, "Uniform not found Error");
    return location;
}
/*
Uniform upload
*/

extern void shaderUploadUniform1m4(struct n_Window* window, char* name, float floatarray[4])
{
    glUniform4f(getUniform(window, name), floatarray[0], floatarray[1], floatarray[2], floatarray[3]);
}


extern void shaderUploadUniform1f(struct n_Window* window, char* name, float float0)
{
    glUniform1f(getUniform(window, name), float0);
}