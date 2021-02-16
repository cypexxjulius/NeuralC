#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "error.h"

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


    if(!glfwInit())
    {
        ASSERT(0, "Failed to initialize GLFW");
    }

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
    setVSync(1);
    return window;
}