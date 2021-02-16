#include "system.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

extern void n_printSystemInformation()
{

    // Printing system informations
    puts("System Information:");
    printf("    Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("    Version:  %s\n", glGetString(GL_VERSION));
    printf("    Renderer: %s\n", glGetString(GL_RENDER));

}