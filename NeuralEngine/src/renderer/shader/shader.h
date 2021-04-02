#ifndef __SHADER_H_
#define __SHADER_H_

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>

#include <glad/glad.h>

struct Window;

typedef struct Shader
{
    unsigned int ShaderID;
    char name[50];
} Shader;


extern Shader* NewShader(char* ShaderName, char* ShaderPath);


static inline void shaderBind(Shader* this)
{   
    glUseProgram(this->ShaderID);
}   

static inline void shaderUnbind()
{
    glUseProgram(0);
}

static inline void DeleteShader(Shader* this)
{
    glDeleteProgram(this->ShaderID);
}


/*
Uniforms 
*/

extern int getUniform(Shader* this, char *name);

static inline void shaderUploadUniform1m4(Shader* this, char* name, mat4 matrix)
{
    glUniformMatrix4fv(getUniform(this, name), 1, GL_FALSE, (const GLfloat *)matrix);
}

static inline void shaderUploadUniform1f(Shader* this, char* name, float float0)
{
    glUniform1f(getUniform(this, name), float0);
}

static inline void shaderUploadUniform1i(Shader* this, char* name, int number)
{
    glUniform1i(getUniform(this, name), number);
}

static inline void shaderUploadUniform4f(Shader* this, char* name, float x, float y, float z, float w)
{
    glUniform4f(getUniform(this, name), x, y, z, w);
}

#endif // __SHADER_H_