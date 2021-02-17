#include "event.h"

#include <stdlib.h>
#include "../core/window.h"

inline static void dispatchEvent(struct n_Window* this, n_EventDispatcher* event)
{
    vectorAdd(this->eventQueue, event);
}

static void windowSizeCallback(GLFWwindow* glWindow, int width, int height)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    

    dispatcher->type = WINDOW_RESIZE_EVENT;

    dispatcher->windowResizeEvent.height = window->height = height;
    dispatcher->windowResizeEvent.width  = window->width  = width;

    dispatchEvent(window, dispatcher);
}

static void windowCloseCallback(GLFWwindow* glWindow)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);
    window->shouldClose = 1;

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    
    dispatcher->type = WINDOW_CLOSE_EVENT;

    dispatchEvent(window, dispatcher);
}

static void keyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    

    dispatcher->type = KEY_CALLBACK_EVENT;
    
    dispatcher->keyEvent.keyCode = key;
    dispatcher->keyEvent.action = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    dispatchEvent(window, dispatcher);
}

static void charCallback(GLFWwindow* glWindow, unsigned int key)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    
    
    dispatcher->type = CHAR_CALLBACK_EVENT;

    dispatcher->keyEvent.keyCode = key;

    dispatchEvent(window, dispatcher);
}

static void mouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    
    
    dispatcher->type = MOUSE_BUTTON_EVENT;

    dispatcher->keyEvent.keyCode = button;
    dispatcher->keyEvent.action = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;

    dispatchEvent(window, dispatcher);
}

static void scrollCallback(GLFWwindow* glWindow, double xOffset, double yOffset)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    
    
    dispatcher->type = SCROLL_EVENT;

    dispatcher->posEvent.pos[0]  = (float)xOffset;
    dispatcher->posEvent.pos[1]  = (float)yOffset;

    dispatchEvent(window, dispatcher);
}

static void mouseMoveCallback(GLFWwindow* glWindow, double x, double y)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    n_EventDispatcher* dispatcher = malloc(sizeof(n_EventDispatcher));    
    
    dispatcher->type = MOUSE_MOVED_EVENT;


    window->mouse.pos[0] = dispatcher->posEvent.pos[0]  = (float)x;
    window->mouse.pos[1] = dispatcher->posEvent.pos[1]  = (float)y;

    dispatchEvent(window, dispatcher);

    
}



// Callback Functions
void n_initEvent(n_Window* this)
{
    glfwSetWindowSizeCallback(this->windowHandle, windowSizeCallback);
    glfwSetWindowCloseCallback(this->windowHandle, windowCloseCallback);
    glfwSetKeyCallback(this->windowHandle, keyCallback);
    glfwSetCharCallback(this->windowHandle, charCallback);
    glfwSetMouseButtonCallback(this->windowHandle, mouseButtonCallback);
    glfwSetCursorPosCallback(this->windowHandle, mouseMoveCallback);
    glfwSetScrollCallback(this->windowHandle, scrollCallback); 
}

