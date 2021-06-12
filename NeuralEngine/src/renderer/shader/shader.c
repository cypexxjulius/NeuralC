#include "shader.h"

#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/core/window.h"
#include "src/platform/memory.h"
#include "src/utils/types.h"
#include "src/core/error.h"

#include "cglm/common.h"

#include <glad/glad.h>
#include <string.h>
#include <stdio.h>

typedef enum ShaderType
{
    ShaderTypeNone = 0,
    VertexShaderType = 1,
    FragmentShaderType = 2
} ShaderType;

extern void ShaderBind(Shader *this)
{
    persist u32 BoundShaderID = 0;
    if (this->ShaderID == BoundShaderID)
        return;

    glUseProgram(this->ShaderID);
    BoundShaderID = this->ShaderID;
}

static int ShaderPreProcessAssetFile(String File, String outFiles[2])
{
    int StartPosition = StringContainsString(File, String("#type")) + 5; // Get first #type in file; 5 for strlen("#type")

    if (StartPosition == -1) // Return Error Code if no #type definition is found
        goto errorCleanup;

    while (StartPosition != -1)
    {
        File = StringResize(File, StartPosition);
        int lineLength = StringContainsChar(File, '\n');

        if (lineLength == -1)
            goto errorCleanup;

        ShaderType typeOfShader = ShaderTypeNone;
        unsigned int i = 0;

        while (File.string[i] != '\0' || File.string[i] != '\n') // Iterate through the rest of the line
        {
            if (File.string[i] == ' ') // I dont care about white spaces
            {
                i++;
                continue;
            }

            if (lineLength - i >= StringLiteralLength("vertex") && Memory.Compare(File.string + i, "vertex", StringLiteralLength("vertex")) == 0)
            {
                typeOfShader = VertexShaderType;
                break;
            }

            if (lineLength - i >= StringLiteralLength("fragment") && Memory.Compare(File.string + i, "fragment", StringLiteralLength("fragment")) == 0)
            {
                typeOfShader = FragmentShaderType;
                break;
            }

            goto errorCleanup;
        }

        if (typeOfShader == ShaderTypeNone)
        {
            goto errorCleanup; // Could not find a shader type in the file
        }
        // We have found the ShaderType

        File = StringResize(File, lineLength + 1); // Set the "Cursor" to the beginning of the Shader

        int ShaderStringLength = StringContainsString(File, String("#type"));

        if (ShaderStringLength == -1)
        {
            u32 ShaderStringLength = File.length;

            outFiles[typeOfShader - 1].string = Memory.Alloc(ShaderStringLength + 1);

            memcpy(outFiles[typeOfShader - 1].string, File.string, ShaderStringLength);

            outFiles[typeOfShader - 1].string[ShaderStringLength] = '\0'; // Null terminating string
            outFiles[typeOfShader - 1].length = File.length;

            break;
        }
        ShaderStringLength--; // Setting the length one char before the '#' of "#type"

        outFiles[typeOfShader - 1].string = Memory.Alloc(ShaderStringLength + 1);

        Memory.Copy(outFiles[typeOfShader - 1].string, File.string, ShaderStringLength);
        outFiles[typeOfShader - 1].string[ShaderStringLength] = '\0'; // Null terminating string
        outFiles[typeOfShader - 1].length = ShaderStringLength;

        File = StringResize(File, ShaderStringLength);
        StartPosition = 1 + sizeof("#type");
    }

    if (outFiles[0].string == NULL || outFiles[1].string == NULL)
        goto errorCleanup;

    // Success return point
    return 0;

errorCleanup:

    // Check if both fragment and vertex shader have been extracted sucessfully

    if (outFiles[0].string != NULL)
        Memory.Free(outFiles[0].string);

    if (outFiles[1].string != NULL)
        Memory.Free(outFiles[1].string);

    return -1;
}

static int CompileShader(const char *shaderSrc, ShaderType type)
{
    GLenum glType = (type == VertexShaderType) ? GL_VERTEX_SHADER : (type == FragmentShaderType) ? GL_FRAGMENT_SHADER
                                                                                                 : GL_FALSE;

    unsigned int id = glCreateShader(glType);

    glShaderSource(id, 1, (const char **)&shaderSrc, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *message = Memory.Alloc(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        fprintf(stderr, "[SHADER COMPILATION ERROR] %s\n%s\n", (type == VertexShaderType) ? "Vertex" : "Fragment", message);

        Memory.Free(message);
        return -1;
    }

    return id;
}

static Shader *NewShader(const String ShaderName, const char *fragmentShader, const char *vertexShader)
{
    Shader *this = CreateObject(Shader);

    Assert(ShaderName.length > 49, "Shadername to long");              // Checking for the shader name to be under 50
    Memory.Copy(this->name, ShaderName.string, ShaderName.length + 1); // Copying the shader Name

    int vertexShaderID = CompileShader(vertexShader, VertexShaderType);
    int fragmentShaderID = CompileShader(fragmentShader, FragmentShaderType);

    if (vertexShaderID == -1 || fragmentShaderID == -1)
    {
        return NULL;
    }

    this->ShaderID = glCreateProgram();

    glAttachShader(this->ShaderID, vertexShaderID);
    glAttachShader(this->ShaderID, fragmentShaderID);

    glLinkProgram(this->ShaderID);

    int isLinked;
    glGetProgramiv(this->ShaderID, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE)
    {
        int errorMessageLength = 0;
        glGetProgramiv(this->ShaderID, GL_INFO_LOG_LENGTH, &errorMessageLength);

        char *errorMessage = Memory.Alloc(errorMessageLength);

        glGetProgramInfoLog(this->ShaderID, errorMessageLength, &errorMessageLength, errorMessage);
        glDeleteProgram(this->ShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        printf(errorMessage);

        return NULL;
    }

    glDetachShader(this->ShaderID, vertexShaderID);
    glDetachShader(this->ShaderID, fragmentShaderID);

    glValidateProgram(this->ShaderID);

    return this;
};

Shader *NewShaderFromString(const String ShaderName, const char *fragmentShader, const char *vertexShader)
{
    return NewShader(ShaderName, fragmentShader, vertexShader);
};

Shader *NewShaderFromFile(const String ShaderName, char *ShaderPath)
{
    String ShaderSrc = ReadStringFromFile(ShaderPath);

    String ShaderSources[2];

    int status = ShaderPreProcessAssetFile(ShaderSrc, ShaderSources);

    Memory.Free(ShaderSrc.string);

    Assert(status == -1, "Failed to parse Shader");
    
    char *vertexShader = ShaderSources[VertexShaderType - 1].string;
    char *fragmentShader = ShaderSources[FragmentShaderType - 1].string;

    Shader *shader = NewShader(ShaderName, fragmentShader, vertexShader);
    Assert(shader == NULL, "Failed to create Shader");


    Memory.Free(vertexShader);
    Memory.Free(fragmentShader);

    return shader;
}

void DeleteShader(Shader *this)
{
    glDeleteProgram(this->ShaderID);
}

/*
Uniforms
*/

static inline int ShaderGetUniform(Shader *this, char *name)
{

    int location = glGetUniformLocation(this->ShaderID, name);

    if (location == -1)
    {
        char errorMessage[300];
        snprintf((char *const)errorMessage, 300, "Uniform '%s' not found", name);
        Assert(1, errorMessage);
        return -1;
    }
    return location;
}

void ShaderSetFloat4(Shader *this, char *name, v4 vec4)
{
    glUniform4f(ShaderGetUniform(this, name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderSetIntArray(Shader *this, char *name, int *values, u32 count)
{
    glUniform1iv(ShaderGetUniform(this, name), count, values);
}

void ShaderSetMat4(Shader *this, char *name, mat4 matrix)
{
    glUniformMatrix4fv(ShaderGetUniform(this, name), 1, GL_FALSE, (const GLfloat *)matrix);
}

void ShaderSetFloat(Shader *this, char *name, float float0)
{
    glUniform1f(ShaderGetUniform(this, name), float0);
}

void ShaderSetInt(Shader *this, char *name, int number)
{
    glUniform1i(ShaderGetUniform(this, name), number);
}