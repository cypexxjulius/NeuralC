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
    // Convert message from type char* to type wchar_t* 
    unsigned int strLen = strlen(message) + 1;
    wchar_t *wmessage = MemAlloc(sizeof(wchar_t) * strLen);

    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wmessage, strLen, message, _TRUNCATE);

    //Convert file from type char* to type wchar_t*
    strLen = strlen(file) + 1;
    wchar_t *wfile = MemAlloc(sizeof(wchar_t) * strLen);

    convertedChars = 0;
    mbstowcs_s(&convertedChars, wfile, strLen, file, _TRUNCATE);

    _wassert(wmessage, wfile, line);
#else
    __assert(message, file, line);
#endif
}


void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
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
    glDebugMessageCallback(errorCallback, 0);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,&unusedIds,1);
    glfwSetErrorCallback(glfwErrorCallback);
}
