#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wchar.h>

#include "src/platform/memory.h"

void ASSERTImpl(char *message, char *file, unsigned int line)
{
#ifdef _WIN32
    printf("[ASSERTION TRIGGERED] In %s, %i\n%s\n", file, line, message);
    exit(1);
#else
    __assert(message, file, line);
#endif
}
static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "[GLFW ERROR] %i\n%s", error, description);
}

void InitError()
{
    glfwSetErrorCallback(glfwErrorCallback);
}
