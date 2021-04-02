#include "event.h"

#include <stdio.h>
#include "src/core/window.h"

static void _WindowSizeCallback(GLFWwindow* glWindow, int width, int height)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.height = height;
    window->state.width = width;
    glViewport(0, 0, width, height);

    Event event = Event(WindowResizeEventType, .WindowResizeEvent = WindowResizeEvent(width, height));

    if(window->EventCallback)
        window->EventCallback(&event);

}

static void _WindowCloseCallback(GLFWwindow* glWindow)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    Event event = Event(WindowCloseEventType, .close = 0);

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _KeyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.keyboard.keys[key].down = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    Event event = Event(KeyPressedEventType, .KeyPressedEvent = KeyPressedEvent(key, action, mods));

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _MouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    window->state.mouse.buttons[button].down = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    Event event = Event(MouseButtonPressedEventType, .KeyPressedEvent = KeyPressedEvent(button, action, mods));

    if(window->EventCallback)
        window->EventCallback(&event);
}

static void _ScrollCallback(GLFWwindow* glWindow, double _xOffset, double _yOffset)
{
    Window* window = glfwGetWindowUserPointer(glWindow);


    Event event = Event(ScrolledEventType, .PosEvent = PosEvent(v2((float)_xOffset, (float)_yOffset), v2( 0,0 )));

    if(window->EventCallback)
        window->EventCallback(&event);



}

static void _MouseMoveCallback(GLFWwindow* glWindow, double _x, double _y)
{
    static float xOldPos = 0, yOldPos = 0; 

    Window* window = glfwGetWindowUserPointer(glWindow);

    Event event = Event(MouseMovedEventType, .PosEvent = PosEvent( v2((float) _x, (float) _y), v2( (float)_x - xOldPos, (float)_y - yOldPos)));

    window->state.mouse.position = v2((float)_x,(float) _y);

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
}

