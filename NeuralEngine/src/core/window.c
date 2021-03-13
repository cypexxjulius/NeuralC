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
    Window *window = nl_malloc(1 * sizeof(Window));
    memset(window, 0 , sizeof(Window));
    // Storing the width and the height of the window 
    window->state.height = height;
    window->state.width = width;

    // Storing the title
    window->state.title = nl_malloc(strlen(title) + 1);
    memcpy(window->state.title, title, strlen(title) + 1);

    // Initializing event queue


    if(!glfwInit())
    {
        ASSERT(0, "Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window->windowHandle)
    {   
        glfwTerminate();
        ASSERT(0, "Failed to initialize Window");
    }

    glfwMakeContextCurrent(window->windowHandle);


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "%s",  "error initializing GLAD\n");
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(window->windowHandle, window);
    glfwSwapInterval(1);

    LocalWindow = window;
    return window;
}

extern void deleteWindow(Window* window)
{
    glfwTerminate();
    nl_free(window->state.title);
    nl_free(window);
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
    ASSERT(LocalWindow, "Window requested without context");
    return LocalWindow;
}