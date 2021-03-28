#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "error.h"
#include "src/events/event.h"
#include "src/platform/memory.h"


static Window* LocalWindow = NULL;

extern Window* CreateWindow(int width, int height, char *title)
{
    // Initalizing the Window struct
    Window *window = MemAlloc(1 * sizeof(Window));
    memset(window, 0 , sizeof(Window));
    // Storing the width and the height of the window 
    window->state.height = height;
    window->state.width = width;

    // Storing the title
    window->state.title = MemAlloc(strlen(title) + 1);
    memcpy(window->state.title, title, strlen(title) + 1);


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

    LocalWindow = window;
    return window;
}

extern void deleteWindow(Window* window)
{
    glfwTerminate();
    MemFree(window->state.title);
    MemFree(window);
}

extern v2 GetMousePosition(Window* window)
{
    return window->state.mouse.position;
}

extern int IsButtonPressed(Window* window, int key)
{
    return window->state.keyboard.keys[key].down;
}

extern void SetMouseGrabbed(Window* window, u8 grabbed) 
{
    glfwSetInputMode(window->windowHandle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

extern Window* GetWindow()
{
    Assert(!LocalWindow, "Window requested without previous creation");
    return LocalWindow;
}