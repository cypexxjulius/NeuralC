#include "event.h"

#include <stdio.h>
#include "src/core/window.h"
#include "src/utils/types.h"

static u32 id = 1;

static void _WindowSizeCallback(GLFWwindow* glWindow, int width, int height)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.height = height;
    window->state.width = width;

    Event event = {
        .type = WindowResizeEventType,
        .WindowResizeEvent = {
            .height = height,
            .width = width
        },
        .id = id++,
        .Cancable = false
    };
    
    if(window->EventCallback)
        window->EventCallback(&event);

}

static void _WindowCloseCallback(GLFWwindow* glWindow)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    Event event = {
        .type = WindowCloseEventType,
        .Cancable = false,
        .id = id++
    };

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _CharCallback(GLFWwindow* glWindow, unsigned int chara)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    Event event = {
        .type = CharEventType,
        .id = id++, 
        .KeyPressedEvent = { .keyCode = chara },
        .Cancable = true
    };


    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _KeyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.keyboard.keys[key].down = action;

    Event event = { 
        .type = KeyPressedEventType,
        .id = id++,
        .KeyPressedEvent = {
            .keyCode = key, 
            .action = action,
            .mod =  mods
        },
        .Cancable = true 
    };

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _MouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    window->state.mouse.buttons[button].down = action;

    Event event = {
        .type = MouseButtonPressedEventType, 
        .id = id++,
        .KeyPressedEvent = { 
            .keyCode = button, 
            .action = action, 
            .mod = mods, 
            .position = window->state.mouse.position
        }, 
        .Cancable = true
    };

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _ScrollCallback(GLFWwindow* glWindow, double _xOffset, double _yOffset)
{
    Window* window = glfwGetWindowUserPointer(glWindow);


    Event event = {
        .type = ScrolledEventType,
        .id = id++,
        .PosEvent = { 
            V2( (float)_xOffset, (float)_yOffset ), 
            V2( 0,0 ), 
        },
        .Cancable = true
    };

    if(window->EventCallback)
        window->EventCallback(&event);



}

static void _MouseMoveCallback(GLFWwindow* glWindow, double _x, double _y)
{
    static float xOldPos = 0, yOldPos = 0; 

    Window* window = glfwGetWindowUserPointer(glWindow);

    Event event = {
        .type = MouseMovedEventType, 
        .id = id++,
        .PosEvent = {
            V2((float)_x,           (float)_y), 
            V2((float)_x - xOldPos, (float)_y - yOldPos)
        }, 
        .Cancable = true
    };
    
    window->state.mouse.position = V2((float)_x,(float) _y);

    xOldPos = (float)_x;
    yOldPos = (float)_y;


    if(window->EventCallback)
        window->EventCallback(&event);
}



// Callback Functions
void InitEventSystem(Window* this)
{
    glfwSetFramebufferSizeCallback(this->windowHandle, _WindowSizeCallback);
    glfwSetWindowCloseCallback(this->windowHandle, _WindowCloseCallback);
    glfwSetKeyCallback(this->windowHandle, _KeyCallback);
    glfwSetMouseButtonCallback(this->windowHandle, _MouseButtonCallback);
    glfwSetCursorPosCallback(this->windowHandle, _MouseMoveCallback);
    glfwSetScrollCallback(this->windowHandle, _ScrollCallback); 
    glfwSetCharCallback(this->windowHandle, _CharCallback);
}

