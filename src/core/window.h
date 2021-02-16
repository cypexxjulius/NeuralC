#ifndef __WINDOW_H_
#define __WINDOW_H_


#include "opengl.h"
#include "../events/event.h"

typedef struct 
{
    GLFWwindow* windowHandle;
    char *title;
    unsigned int width;
    unsigned int height;
    unsigned char shouldClose;
    unsigned int shader;

    // Events
    n_Event events;

} n_Window;

extern n_Window* n_createWindow(int width, int height, char *title);

#endif // __WINDOW_H_