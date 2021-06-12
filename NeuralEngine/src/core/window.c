#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "error.h"
#include "src/events/event.h"
#include "src/platform/memory.h"


Window* NewWindow(int width, int height, char *title)
{
    // Initalizing the Window struct
    Window *window = CreateObject(Window);

    // Storing the width and the height of the window 
    window->state.height = height;
    window->state.width = width;

    // Storing the title
    window->state.title = Memory.Alloc(strlen(title) + 1);
    Memory.Copy(window->state.title, title, strlen(title) + 1);


    if(!glfwInit())
    {
        Assert(1, "Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window->windowHandle)
    {   
        glfwTerminate();
        Assert(1, "Failed to initialize Window");
    }

    glfwMakeContextCurrent(window->windowHandle);


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        Assert(1, "Program failed to initializing GLAD");
    }

    glfwSetWindowUserPointer(window->windowHandle, window);
    glfwSwapInterval(1);

    return window;
}

void DeleteWindow(Window* window)
{
    glfwTerminate();
    Memory.Free(window->state.title);
    Memory.Free(window);
}

void WindowSwapBuffers(Window* window)
{
    glfwSwapBuffers(window->windowHandle);
}

void WindowPollEvents()
{
    // Poll Events
    glfwPollEvents();
}