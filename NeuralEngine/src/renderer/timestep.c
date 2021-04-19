#include "timestep.h"

#include <GLFW/glfw3.h>

static float last_time;

inline float GetDeltaTime()
{
    float time = (float)glfwGetTime(); 
    float deltaTime = time - last_time;
    last_time = time;
    return deltaTime;
}


