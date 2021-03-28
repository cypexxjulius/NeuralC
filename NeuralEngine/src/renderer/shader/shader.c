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
} ShaderType;


static int CompileShader(char *shaderSrc, ShaderType type)
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
        fprintf(stderr, "%s shader failed to compile\n", (type == VertexShaderType) ? "Vertex" : "Fragment");
        fprintf(stderr, "[ERROR MESSAGE]\n%s\n", message);

        glGetShaderInfoLog(id, length, &length, message);


        MemFree(message);
        return -1 ;
    }   

    return id;
}

extern Shader* newShader(char *ShaderName, char* vertexShaderPath, char* fragmentShaderPath)
{
    Shader* this = CreateObject(Shader);
  
    Assert(strlen(ShaderName) > 49, "Shadername to long");  // Checking for the shader name to be under 50 
    MemCpy(this->name, ShaderName, strlen(ShaderName) + 1);     // Copying the shader Name
    
    char *vertexShader    = ReadStringFromFile(vertexShaderPath);
    char *fragmentShader  = ReadStringFromFile(fragmentShaderPath);    

    this->ShaderID = glCreateProgram();

    unsigned int vertexShaderID     = CompileShader(vertexShader, VertexShaderType);
    unsigned int fragmentShaderID   = CompileShader(fragmentShader, FragmentShaderType);

    if(vertexShaderID == -1 || fragmentShaderID == -1)
    {
        return NULL;
    }

    MemFree(vertexShader);
    MemFree(fragmentShader);

    glAttachShader(this->ShaderID, vertexShaderID);
    glAttachShader(this->ShaderID, fragmentShaderID);

    glLinkProgram(this->ShaderID);

    int isLinked;
    glGetProgramiv(this->ShaderID, GL_LINK_STATUS, &isLinked);

    if(isLinked == GL_FALSE)
    {
        unsigned int errorMessageLength = 0;
        glGetProgramiv(this->ShaderID, GL_INFO_LOG_LENGTH, &errorMessageLength);

        char* errorMessage = MemAlloc(errorMessageLength);
        Assert(!errorMessage, "Memory allocation failed");

        
        glDeleteProgram(this->ShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        CoreWarn(errorMessage);

        return NULL;
    }

    glDetachShader(this->ShaderID, vertexShaderID);
    glDetachShader(this->ShaderID, fragmentShaderID);


    glValidateProgram(this->ShaderID);

    return this;
}

extern void shaderBind(Shader* this)
{   
    glUseProgram(this->ShaderID);
}   

extern void shaderUnbind()
{
    glUseProgram(0);
}

extern void deleteShader(Shader* this)
{
    glDeleteProgram(this->ShaderID);
}


/*
Uniforms
*/

static inline int getUniform(Shader* this, char *name)
{

    int location = glGetUniformLocation(this->ShaderID, name);
    if(location != -1)
    {
        char errorMessage[300];
        snprintf(errorMessage, 300, "Uniform '%s' not found", name);
        Assert(location == -1, errorMessage);
    }
    return location;
}


extern void shaderUploadUniform1m4(Shader* this, char* name, mat4 matrix)
{
    glUniformMatrix4fv(getUniform(this, name), 1, GL_FALSE, (const GLfloat *)matrix);
}

extern void shaderUploadUniform1f(Shader* this, char* name, float float0)
{
    glUniform1f(getUniform(this, name), float0);
}

extern void shaderUploadUniform1i(Shader* this, char* name, int number)
{
    glUniform1i(getUniform(this, name), number);
}