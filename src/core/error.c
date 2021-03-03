#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include "opengl.h"


void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    source = source;
    length = length;
    userParam = userParam;
    id = id;
    fprintf(stderr, "[OPENGL ERROR]:\n" 
                    "Source     : 0x%x\n"
                    "Type       : 0x%x\n"
                    "Severity   : 0x%x\n"
                    "ERROR:\n%s\n", source, type, severity, message);
}

static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "[GLFW ERROR] %i\n%s", error, description);
}

void n_initError()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorCallback, 0);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,&unusedIds,1);
    glfwSetErrorCallback(glfwErrorCallback);
}

extern void printAssertMessageAndExit(char *message, char * file, int line)
{
    fprintf(stderr, "[ERROR] %s %d\n%s\n", file, line, (message)); exit(1);
}