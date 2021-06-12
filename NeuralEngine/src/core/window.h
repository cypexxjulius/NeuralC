#ifndef __WINDOW_H_
#define __WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/events/eventStructs.h"
#include "src/utils/types.h"

struct ButtonData {
    u32 down, last, last_tick, pressed, pressed_tick;
};

struct MouseData {
    struct ButtonData buttons[GLFW_MOUSE_BUTTON_LAST];
    v2 position; 
    float delta[2], scrollOffset[2];
};

struct KeyboardData {
    struct ButtonData keys[GLFW_KEY_LAST];
};

typedef struct Window
{
    GLFWwindow* windowHandle;

    void (*EventCallback)(Event* event);
    struct state 
    {
        char *title;
        unsigned int width;
        unsigned int height;

        // State 
        struct MouseData mouse;
        struct KeyboardData keyboard;
    } state;

} Window;


extern Window* NewWindow(int width, int height, char *title);

extern void DeleteWindow(Window* window);

extern void WindowSwapBuffers(Window* window);

extern void WindowPollEvents();

#endif // __WINDOW_H_