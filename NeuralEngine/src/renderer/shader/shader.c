#include "shader.h"

#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/core/window.h"
#include "src/platform/memory.h"
#include "src/utils/types.h"
#include "src/core/error.h"

#include "cglm/common.h"

#include <string.h>
#include <stdio.h>

typedef enum ShaderType 
{
    ShaderTypeNone = 0,
    VertexShaderType = 1,
    FragmentShaderType = 2
} ShaderType;

extern void ShaderBind(Shader* this)
{   
    static u32 BoundShaderID = 0;
    if(this->ShaderID == BoundShaderID)
        return;
    
    glUseProgram(this->ShaderID);
    BoundShaderID = this->ShaderID;
}   


static int ShaderPreProcessAssetFile(char *File, char *outFiles[2])
{
    outFiles[0] = NULL;
    outFiles[1] = NULL;
 
    int StartPosition = StringContainsString(File, "#type") + 5;// Get first #type in file; 5 for strlen("#type")

    if(StartPosition == -1)                                 // Return Error Code if no #type definition is found
        goto errorCleanup;          

    while(StartPosition != -1)
    {
        File += StartPosition;
        int lineLength = StringContainsChar(File, '\n');

        if(lineLength == -1)
            goto errorCleanup;        

        ShaderType typeOfShader = ShaderTypeNone;
        unsigned int i = 0;

        while(File[i] != '\0' || File[i] != '\n') // Iterate through the rest of the line
        {
            if(File[i] == ' ')       // I dont care about white spaces
            {
                i++;                                    
                continue;
            }

            if(lineLength - i >= StringLiteralLength("vertex") && Memory.Compare(File + i, "vertex", StringLiteralLength("vertex")) == 0)
            {
                typeOfShader = VertexShaderType;
                break;
            }
            
            if(lineLength - i >= StringLiteralLength("fragment") && Memory.Compare(File + i, "fragment", StringLiteralLength("fragment")) == 0)
            {
                typeOfShader = FragmentShaderType;
                break;
            }

            goto errorCleanup;
        }
        

        if(typeOfShader == ShaderTypeNone)
        {
            goto errorCleanup; // Could not find a shader type in the file
        }
        // We have found the ShaderType 

        File += lineLength + 1; // Set the "Cursor" to the beginning of the Shader

        int ShaderStringLength = StringContainsString(File, "#type"); 

        if(ShaderStringLength == -1)
        {
            size_t ShaderStringLength  = strlen(File);

            outFiles[typeOfShader - 1] = Memory.Alloc(ShaderStringLength + 1);

            Memory.Copy(outFiles[typeOfShader - 1], File, ShaderStringLength); 
        
            outFiles[typeOfShader - 1][ShaderStringLength] = '\0';      // Null terminating string

            break;
        }
        ShaderStringLength--; // Setting the length one char before the '#' of "#type"
    
        outFiles[typeOfShader - 1] = Memory.Alloc(ShaderStringLength + 1);

        Memory.Copy(outFiles[typeOfShader - 1], File, ShaderStringLength);
        outFiles[typeOfShader - 1][ShaderStringLength] = '\0';      // Null terminating string

        File += ShaderStringLength;
        StartPosition = 1 + sizeof("#type"); 
    }

    if(outFiles[0] == NULL || outFiles[1] == NULL)
        goto errorCleanup;

    
    // Success return point
    return 0;


    errorCleanup:

    // Check if both fragment and vertex shader have been extracted sucessfully

    if(outFiles[0] != NULL)
        Memory.Free(outFiles[0]);
    
    else if(outFiles[1] != NULL)
        Memory.Free(outFiles[1]);

    return -1;
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

        char *message = Memory.Alloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);


        fprintf(stderr, "[SHADER COMPILATION ERROR] %s\n%s\n",(type == VertexShaderType) ? "Vertex" : "Fragment", message);



        Memory.Free(message);
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

    Memory.Free(ShaderSrc);
    
    if(status == -1)
        return NULL;

  
    Assert(strlen(ShaderName) > 49, "Shadername to long");  // Checking for the shader name to be under 50 
    Memory.Copy(this->name, ShaderName, strlen(ShaderName) + 1);     // Copying the shader Name
    
    char *vertexShader    = ShaderSources[VertexShaderType - 1];
    char *fragmentShader  = ShaderSources[FragmentShaderType - 1];    

    //printf("VertexShader\n---\n%s\n---\n", vertexShader);
    //printf("FragmentShader\n---\n%s\n---\n", fragmentShader);

    this->ShaderID = glCreateProgram();

    int vertexShaderID     = CompileShader(vertexShader, VertexShaderType);
    int fragmentShaderID   = CompileShader(fragmentShader, FragmentShaderType);

    if(vertexShaderID == -1 || fragmentShaderID == -1)
    {
        return NULL;
    }

    Memory.Free(vertexShader);
    Memory.Free(fragmentShader);

    glAttachShader(this->ShaderID, vertexShaderID);
    glAttachShader(this->ShaderID, fragmentShaderID);

    glLinkProgram(this->ShaderID);

    int isLinked;
    glGetProgramiv(this->ShaderID, GL_LINK_STATUS, &isLinked);

    if(isLinked == GL_FALSE)
    {
        int errorMessageLength = 0;
        glGetProgramiv(this->ShaderID, GL_INFO_LOG_LENGTH, &errorMessageLength);

        char* errorMessage = Memory.Alloc(errorMessageLength);
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

extern int ShaderGetUniform(Shader* this, char *name)
{

    int location = glGetUniformLocation(this->ShaderID, name);

    if(location == -1)
    {
        char errorMessage[300];
        snprintf((char* const)errorMessage, 300, "Uniform '%s' not found", name);
        Assert(1, errorMessage);
        return -1;
    }
    return location;
}