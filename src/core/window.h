#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_


#include "opengl.h"
#include "../events/eventStructs.h"
#include "../utils/types.h"

struct Button {
    uint down, last, last_tick, pressed, pressed_tick;
};

struct n_Mouse {
    struct Button buttons[GLFW_MOUSE_BUTTON_LAST];
    n_Position position; 
    float2 delta, scrollOffset;
};

struct n_Keyboard {
    struct Button keys[GLFW_KEY_LAST];
};

struct n_Char {
    uint key[256];
};

typedef struct n_Window
{
    GLFWwindow* windowHandle;
    char *title;
    unsigned int width;
    unsigned int height;
    unsigned char shouldClose;
    unsigned int shader;

    // State 
    struct n_Mouse mouse;
    struct n_Keyboard keyboard;
    struct n_Char charSet;


} n_Window;


extern n_Window* n_createWindow(int width, int height, char *title);

extern void deleteWindow(n_Window* window);

extern n_Position n_getMousePosition(n_Window* window);

extern int n_isButtonPressed(n_Window* window, char key);



#endif // _CORE_WINDOW_H_