#include <string.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "window.h"
#include "error.h"
#include "src/events/event.h"
#include "src/platform/memory.h"


extern n_Window* n_createWindow(int width, int height, char *title)
{
    // Initalizing the n_window struct
    n_Window *window = nl_malloc(1 * sizeof(n_Window));
    memset(window, 0 , sizeof(n_Window));
    // Storing the width and the height of the window 
    window->height = height;
    window->width = width;

    // Storing the title
    window->title = nl_malloc(strlen(title) + 1);
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


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "%s",  "error initializing GLAD\n");
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(window->windowHandle, window);
    glfwSwapInterval(1);


    n_initError(window);

    n_initEvent(window);

    return window;
}

extern void deleteWindow(n_Window* window)
{
    glfwTerminate();
    nl_free(window->title);
    nl_free(window);
}

extern v2 n_getMousePosition(n_Window* window)
{
    return window->mouse.position;
}

extern int isButtonPressed(n_Window* window, int key)
{
    return window->keyboard.keys[key].down;
}
extern void setMouseGrabbed(n_Window* window, int8_t grabbed) 
{
    glfwSetInputMode(window->windowHandle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}