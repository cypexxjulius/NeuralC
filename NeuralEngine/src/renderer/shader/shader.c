#include "shader.h"

#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/core/window.h"
#include "src/platform/memory.h"


#include "cglm/common.h"

#include <stdio.h>

typedef enum ShaderType 
{
    ShaderTypeNone = 0,
    VertexShaderType = 1,
    FragmentShaderType = 2
} ShaderType;

static int ShaderPreProcessAssetFile(char *File, char *outFiles[2])
{
    outFiles[0] = NULL;
    outFiles[1] = NULL;
 
    int StartPosition = StringContainsString(File, "#type") + 5;// Get first #type in file; 5 for strlen("#type")

    if(StartPosition == -1)                                 // Return Error Code if no #type definition is found
        return -1;          

    while(StartPosition != -1)
    {
        File += StartPosition;
        int lineLength = StringContainsChar(File, '\n');

        if(lineLength == -1)
            return -1;        

        ShaderType typeOfShader = ShaderTypeNone;
        unsigned int i = 0;
        while(File[i] != '\0' || File[i] != '\n') // Iterate through the rest of the line
        {
            if(File[i] == ' ')       
            {
                i++;                                    // I dont care about white spaces
                continue;
            }

            if(lineLength - i >= strlen("vertex") && MemCmp(File + i, "vertex", strlen("vertex")) == 0)
            {
                typeOfShader = VertexShaderType;
                break;
            }
            
            if(lineLength - i >= strlen("fragment") && MemCmp(File + i, "fragment", strlen("fragment")) == 0)
            {
                typeOfShader = FragmentShaderType;
                break;
            }
            
            return -1;
        }

        if(typeOfShader == ShaderTypeNone)
            return -1;

        StartPosition = lineLength;

        int EndPosition = StringContainsString(File + StartPosition, "#type"); 
        if(EndPosition == -1)
        {
            EndPosition = strlen(File);
        
            unsigned int ShaderStringLength = EndPosition - StartPosition;

            outFiles[typeOfShader - 1] = MemAlloc(ShaderStringLength + 1);

            MemCpy(outFiles[typeOfShader - 1], File + StartPosition, ShaderStringLength);
        
            outFiles[typeOfShader - 1][ShaderStringLength] = '\0';      // Null terminating string

            break;
        }

        unsigned int ShaderStringLength = EndPosition;

        outFiles[typeOfShader - 1] = MemCalloc(ShaderStringLength + 1, sizeof(char));

        MemCpy(outFiles[typeOfShader - 1], File + StartPosition, ShaderStringLength);
        
        
        File += StartPosition + EndPosition;
        StartPosition = StringContainsString(File, "#type") + 5;
    }
    if(outFiles[0] == NULL || outFiles[1] == NULL)
    {
        if(outFiles[0] != NULL)
        {
            MemFree(outFiles[0]);
        }
        else
        {
            MemFree(outFiles[1]);
        }
    }
    return 0;
}

static int CompileShader(char *shaderSrc, ShaderType type)
{
    GLenum glType = (type == VertexShaderType) ? GL_VERTEX_SHADER : (type == FragmentShaderType) ? GL_FRAGMENT_SHADER : GL_FALSE;

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
        glGetShaderInfoLog(id, length, &length, message);


        fprintf(stderr, "[SHADER COMPILATION ERROR] %s\n%s\n",(type == VertexShaderType) ? "Vertex" : "Fragment", message);



        MemFree(message);
        return -1 ;
    }   

    return id;
}

extern Shader* NewShader(char *ShaderName, char* ShaderPath)
{
    Shader* this = CreateObject(Shader);

    char *ShaderSrc = ReadStringFromFile(ShaderPath);

    char *ShaderSources[2];

    int status = ShaderPreProcessAssetFile(ShaderSrc, ShaderSources);

    if(status == -1)
    {
        MemFree(ShaderSrc);
        return NULL;
    }

    MemFree(ShaderSrc);

  
    Assert(strlen(ShaderName) > 49, "Shadername to long");  // Checking for the shader name to be under 50 
    MemCpy(this->name, ShaderName, strlen(ShaderName) + 1);     // Copying the shader Name
    
    char *vertexShader    = ShaderSources[VertexShaderType - 1];
    char *fragmentShader  = ShaderSources[FragmentShaderType - 1];    

    this->ShaderID = glCreateProgram();

    int vertexShaderID     = CompileShader(vertexShader, VertexShaderType);
    int fragmentShaderID   = CompileShader(fragmentShader, FragmentShaderType);

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
        int errorMessageLength = 0;
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


/*
Uniforms
*/

extern int getUniform(Shader* this, char *name)
{

    int location = glGetUniformLocation(this->ShaderID, name);
    if(location != -1)
    {
        char errorMessage[300];
        snprintf((char* const)errorMessage, 300, "Uniform '%s' not found", name);
        Assert(location == -1, errorMessage);
    }
    return location;
}