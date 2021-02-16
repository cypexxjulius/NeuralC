#include "event.h"

#include "../core/opengl.h"

static void windowSizeCallback(GLFWwindow* glWindow, int width, int height)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->height = window->events.windowEvent.height = height;
    window->width  = window->events.windowEvent.width  = width;
}

static void windowCloseCallback(GLFWwindow* glWindow)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);
    window->shouldClose = 1;
}

static void keyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->events.keyEvent.keyCode = key;
    window->events.keyEvent.action = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;
}

static void charCallback(GLFWwindow* glWindow, unsigned int key)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->events.charEvent.keyCode = key;
}

static void mouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->events.mouseButtonEvent.keyCode = button;
    window->events.mouseButtonEvent.action = (action == GLFW_PRESS) ? 1 : (action == GLFW_REPEAT) ? 2 : 0;
}

static void scrollCallback(GLFWwindow* glWindow, double xOffset, double yOffset)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->events.scrollEvent.pos[0]  = (float)xOffset;
    window->events.scrollEvent.pos[1]  = (float)yOffset;
}

static void scrollCallback(GLFWwindow* glWindow, double x, double y)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->events.mouseMoveEvent.pos[0]  = (float)x;
    window->events.mouseMoveEvent.pos[1]  = (float)y;
}



// Callback Functions

void n_createEvent(n_Window* this)
{
    glfwSetWindowSizeCallback(this->windowHandle)
}

