#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_


#include "window.h"
#include "src/utils/types.h"
#include "src/renderer/timestep.h"

typedef struct Application
{
    n_Window* (*Init)();
    void (*OnUpdate)(float deltaTime, n_Window* window);
    bool (*OnEvent)();
    void (*Delete)();
} Application;

#define Application(_init, _onupdate, _onevent, _delete) (Application){_init, _onupdate, _onevent, _delete }

Application CreateApplication();

int main()
{   
    Application app = CreateApplication();

    n_Window* window = app.Init();

    while(!window->shouldClose)
    {
        float deltaTime = getDeltaTime();
        app.OnUpdate(deltaTime, window);
    }

    app.Delete();

       
    deleteWindow(window);
    return 0;
}



#endif // __ENTRYPOINT_H_