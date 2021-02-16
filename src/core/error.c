#include "error.h"


void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    source = source;
    length = length;
    userParam = userParam;
    id = id;
    fprintf(stderr, "[OPENGL ERROR]:\n" 
                    "Source     : %i\n"
                    "Type       : %i\n"
                    "Severity   : %i\n"
                    "ERROR:\n%s\n", source, type, severity, message);
}

static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "[GLFW ERROR] %i\n%d", error, description);
}

void n_initError()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorCallback, 0);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,&unusedIds,1);
    glfwSetErrorCallback(glfwErrorCallback);
}