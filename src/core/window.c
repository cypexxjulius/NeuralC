#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "error.h"
#include "../events/event.h"

extern n_Window* n_createWindow(int width, int height, char *title)
{
    // Initalizing the n_window struct
    n_Window *window = calloc(1, sizeof(n_Window));
    
    // Storing the width and the height of the window 
    window->height = height;
    window->width = width;

    window->shouldClose = 0;

    // Storing the title
    window->title = malloc(strlen(title) + 1);
    memcpy(window->title, title, strlen(title) + 1);

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


    if(glewInit() != GLEW_OK)
    {
        ASSERT(0, "Failed to initialize GLEW");
    }

    glfwSetWindowUserPointer(window->windowHandle, window);
    glfwSwapInterval(1);

    n_initError(window);

    n_initEvent(window);



    return window;
}

extern n_Position n_getMousePosition(n_Window* window)
{
    return window->mouse.position;
}

extern int n_isButtonPressed(n_Window* window, char key)
{
    return window->keyboard.keys[(int)key].down;
}