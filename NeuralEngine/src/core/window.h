#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/events/eventStructs.h"
#include "src/utils/types.h"

struct Button {
    u32 down, last, last_tick, pressed, pressed_tick;
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
    u32 key[256];
};

typedef struct Window
{
    GLFWwindow* windowHandle;

    bool (*ErrorCallback)(Event* event);
    struct state 
    {
        char *title;
        unsigned int width;
        unsigned int height;
        unsigned char shouldClose;

        // State 
        struct n_Mouse mouse;
        struct n_Keyboard keyboard;
    } state;

} Window;


extern Window* CreateWindow(int width, int height, char *title);

extern void deleteWindow(Window* window);

extern v2 GetMousePosition(Window* window);

extern int IsButtonPressed(Window* window, int key);

extern void SetMouseGrabbed(Window* window, u8 grabbed);

#endif // _CORE_WINDOW_H_