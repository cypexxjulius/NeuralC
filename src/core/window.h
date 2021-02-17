#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_


#include "opengl.h"
#include "../events/eventStructs.h"
#include "../utils/types.h"

struct n_Mouse
{
    float pos[2];
};

typedef struct n_Window
{
    GLFWwindow* windowHandle;
    char *title;
    unsigned int width;
    unsigned int height;
    unsigned char shouldClose;
    unsigned int shader;
    struct n_Mouse mouse;

    // Events
    vector* eventQueue;

} n_Window;

extern n_Window* n_createWindow(int width, int height, char *title);

#endif // _CORE_WINDOW_H_