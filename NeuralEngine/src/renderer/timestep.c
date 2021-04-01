#include "timestep.h"

#include <GLFW/glfw3.h>

static float last_time;

extern float GetDeltaTime()
{
    float time = glfwGetTime(); 
    float deltaTime = time - last_time;
    last_time = time;
    return deltaTime;
}


