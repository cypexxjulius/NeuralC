#include "shader.h"

#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/core/window.h"
#include "src/platform/memory.h"


#include "cglm/common.h"

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

        char *message = MemAlloc(length * sizeof(char));
        printf("Shader: %s\n", (type == VertexShaderType) ? "Vertex shader" : "Fragement shader");
        glGetShaderInfoLog(id, length, &length, message);

        ASSERT(0, message);

        MemFree(message);
    }   

    return id;
}

extern n_Shader newShader(char* vertexShaderPath, char* fragmentShaderPath)
{
    char *vertexShader    = n_readFile(vertexShaderPath);
    char *fragmentShader  = n_readFile(fragmentShaderPath);    

    unsigned int program = glCreateProgram();

    unsigned int vertexShaderID     = compileShader(vertexShader, VertexShaderType);
    unsigned int fragmentShaderID   = compileShader(fragmentShader, FragmentShaderType);


    MemFree(vertexShader);
    MemFree(fragmentShader);

    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);

    glLinkProgram(program);

    glDetachShader(program, vertexShaderID);
    glDetachShader(program, fragmentShaderID);


    glValidateProgram(program);


    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return program;
}

extern void shaderBind(n_Shader this)
{   
    glUseProgram(this);
}   

extern void shaderUnbind()
{
    glUseProgram(0);
}

extern void deleteShader(n_Shader this)
{
    glDeleteProgram(this);
}


/*
Uniforms
*/

static inline int getUniform(n_Shader this, char *name)
{

    int location = glGetUniformLocation(this, name);
    ASSERT(location != -1, "Uniform not found Error");
    return location;
}


extern void shaderUploadUniform1m4(n_Shader this, char* name, mat4 matrix)
{
    glUniformMatrix4fv(getUniform(this, name), 1, GL_FALSE, (const GLfloat *)matrix);
}

extern void shaderUploadUniform1f(n_Shader this, char* name, float float0)
{
    glUniform1f(getUniform(this, name), float0);
}

extern void shaderUploadUniform1i(n_Shader this, char* name, int number)
{
    glUniform1i(getUniform(this, name), number);
}