#include "timestep.h"

#include "../libinclude/glfw.h"

static float last_time;

extern float getDeltaTime()
{
    float time = glfwGetTime(); 
    float deltaTime = time - last_time;
    last_time = time;
    return deltaTime;
}


