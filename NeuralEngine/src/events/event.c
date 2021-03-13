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

    if(window->ErrorCallback)
        window->ErrorCallback(&event);

}

static void _WindowCloseCallback(GLFWwindow* glWindow)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.shouldClose = 1;

    Event event = Event(WindowCloseEventType, .close = 0);

    if(window->ErrorCallback)
        window->ErrorCallback(&event);
}

static void _KeyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);
    window->state.keyboard.keys[key].down = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    Event event = Event(KeyPressedEventType, .KeyPressedEvent = KeyPressedEvent(key, action, mods));

    if(window->ErrorCallback)
        window->ErrorCallback(&event);
}

static void _MouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    window->state.mouse.buttons[button].down = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    Event event = Event(MouseButtonPressedEventType, .KeyPressedEvent = KeyPressedEvent(button, action, mods));

    if(window->ErrorCallback)
        window->ErrorCallback(&event);
}

static void _ScrollCallback(GLFWwindow* glWindow, double _xOffset, double _yOffset)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    static v2 oldPos = {0, 0};

    v2 Offset = v2((float)_xOffset, (float)_yOffset);
    
    v2 Delta = v2(Offset.x - oldPos.x, Offset.y - oldPos.y);

    oldPos = Offset;

    window->state.mouse.scrollOffset[0]  = Offset.x;
    window->state.mouse.scrollOffset[1]  = Offset.y;


    Event event = Event(ScrolledEventType, .PosEvent = PosEvent(Offset, Delta));

    if(window->ErrorCallback)
        window->ErrorCallback(&event);



}

static void _MouseMoveCallback(GLFWwindow* glWindow, double _x, double _y)
{
    Window* window = glfwGetWindowUserPointer(glWindow);

    v2 pos   = v2((float)_x, (float)_y);
    v2 delta = v2(pos.x - window->state.mouse.position.x, pos.y - window->state.mouse.position.y);


    window->state.mouse.position = pos;

    Event event = Event(MouseMovedEventType, .PosEvent = PosEvent(pos, delta));
    
    if(window->ErrorCallback)
        window->ErrorCallback(&event);
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

