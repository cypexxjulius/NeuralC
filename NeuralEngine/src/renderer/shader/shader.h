#ifndef __SHADER_H_
#define __SHADER_H_

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>

#include <glad/glad.h>
#include "src/utils/types.h"
struct Window;

typedef struct Shader
{
    unsigned int ShaderID;
    char name[50];
} Shader;


extern Shader* NewShader(char* ShaderName, char* ShaderPath);


static inline void ShaderBind(Shader* this)
{   
    glUseProgram(this->ShaderID);
}   

static inline void ShaderUnbind()
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

extern int ShaderGetUniform(Shader* this, char *name);

static inline void ShaderUploadUniformMat4(Shader* this, char* name, mat4 matrix)
{
    glUniformMatrix4fv(ShaderGetUniform(this, name), 1, GL_FALSE, (const GLfloat *)matrix);
}

static inline void ShaderUploadUniformFloat(Shader* this, char* name, float float0)
{
    glUniform1f(ShaderGetUniform(this, name), float0);
}

static inline void ShaderUploadUniformInt(Shader* this, char* name, int number)
{
    glUniform1i(ShaderGetUniform(this, name), number);
}

static inline void ShaderUploadUniformFloat4(Shader* this, char* name, v4 vec4)
{
    glUniform4f(ShaderGetUniform(this, name), vec4.x, vec4.y, vec4.z, vec4.w);
}

#endif // __SHADER_H_