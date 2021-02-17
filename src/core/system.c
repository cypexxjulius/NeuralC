#include "system.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

extern void n_printSystemInformation()
{

    // Printing system informations
    puts("System Information:");
    printf("    GPU:                %s\n", glGetString(GL_RENDERER));
    printf("    Vendor:             %s\n", glGetString(GL_VENDOR));
    printf("    OpenGL Version:     %s\n", glGetString(GL_VERSION));

}