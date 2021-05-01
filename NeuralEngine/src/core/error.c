#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wchar.h>

#include "src/platform/memory.h"

static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "[GLFW ERROR] %i\n%s", error, description);
}

void InitError()
{
    glfwSetErrorCallback(glfwErrorCallback);
}
