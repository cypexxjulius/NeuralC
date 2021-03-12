#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/events/eventStructs.h"
#include "src/utils/types.h"

struct Button {
    ui16 down, last, last_tick, pressed, pressed_tick;
};

struct n_Mouse {
    struct Button buttons[GLFW_MOUSE_BUTTON_LAST];
    v2 position; 
    float delta[2], scrollOffset[2];
};

struct n_Keyboard {
    struct Button keys[GLFW_KEY_LAST];
};

struct n_Char {
    ui16 key[256];
};

typedef struct n_Window
{
    GLFWwindow* windowHandle;
    char *title;
    unsigned int width;
    unsigned int height;
    unsigned char shouldClose;

    // State 
    struct n_Mouse mouse;
    struct n_Keyboard keyboard;
    struct n_Char charSet;


} n_Window;


extern n_Window* n_createWindow(int width, int height, char *title);

extern void deleteWindow(n_Window* window);

extern v2 n_getMousePosition(n_Window* window);

extern int isButtonPressed(n_Window* window, int key);

extern void setMouseGrabbed(n_Window* window, int8_t grabbed);

#endif // _CORE_WINDOW_H_