#include "event.h"

#include <stdio.h>
#include <stdlib.h>
#include "../core/window.h"

static void windowSizeCallback(GLFWwindow* glWindow, int width, int height)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);
    window->height = height;
    window->width = width;
    glViewport(0, 0, width, height);
}

static void windowCloseCallback(GLFWwindow* glWindow)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);
    window->shouldClose = 1;
}

static void keyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    if (key < 0) {
        return;
    }

    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->keyboard.keys[key].down = action == GLFW_PRESS ? 1 : 0;
}

static void charCallback(GLFWwindow* glWindow, unsigned int key)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    if(key > 0 && key < 256)
        window->charSet.key[key] = 1;
}

static void mouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->mouse.buttons[button].down = GLFW_PRESS ? 1 : 0;
}

static void scrollCallback(GLFWwindow* glWindow, double xOffset, double yOffset)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);


    window->mouse.scrollOffset[0]  = (float)xOffset;
    window->mouse.scrollOffset[1]  = (float)yOffset;

}

static void mouseMoveCallback(GLFWwindow* glWindow, double x, double y)
{
    n_Window* window = glfwGetWindowUserPointer(glWindow);

    window->mouse.position = (n_Position){(float)x, (float)y};
    
}



// Callback Functions
void n_initEvent(n_Window* this)
{
    glfwSetFramebufferSizeCallback(this->windowHandle, windowSizeCallback);
    glfwSetWindowCloseCallback(this->windowHandle, windowCloseCallback);
    glfwSetKeyCallback(this->windowHandle, keyCallback);
    glfwSetCharCallback(this->windowHandle, charCallback);
    glfwSetMouseButtonCallback(this->windowHandle, mouseButtonCallback);
    glfwSetCursorPosCallback(this->windowHandle, mouseMoveCallback);
    glfwSetScrollCallback(this->windowHandle, scrollCallback); 
}

