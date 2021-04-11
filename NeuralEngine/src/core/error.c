#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wchar.h>

#include "src/platform/memory.h"

void __ASSERT(char *message, char *file, unsigned int line)
{
#ifdef _WIN32
    printf("[ASSERTION TRIGGERED] In %s, %i\n%s\n", file, line, message);
    exit(1);
#else
    __assert(message, file, line);
#endif
}


void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if(severity <= 0x826b)
        return;


    source = source;
    length = length;
    userParam = userParam;
    id = id;
    CoreWarn("[OPENGL ERROR]:\n" 
            "Source     : 0x%x\n"
            "Type       : 0x%x\n"
            "Severity   : 0x%x\n"
            "ERROR:\n%s\n", source, type, severity, message);
}

static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "[GLFW ERROR] %i\n%s", error, description);
}

void InitError()
{
    glEnable(GL_DEBUG_OUTPUT);

    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0,&unusedIds, 1);
    glDebugMessageCallback(errorCallback, 0);
    
    glfwSetErrorCallback(glfwErrorCallback);
}
