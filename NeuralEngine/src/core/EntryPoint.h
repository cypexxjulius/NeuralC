#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_


#include "window.h"
#include "src/utils/types.h"
#include "src/renderer/timestep.h"

typedef struct Application
{
    Window* (*Init)();
    void (*OnUpdate)(float deltaTime, Window* window);
    bool (*OnEvent)(Event* event);
    void (*Delete)();
} Application;

#define Application(_init, _onupdate, _onevent, _delete) (Application){_init, _onupdate, _onevent, _delete }

Application CreateApplication();

int main()
{   
    Application app = CreateApplication();

    Window* window = app.Init();
    window->ErrorCallback = app.OnEvent;

    InitError();

    InitEventSystem(window);

    while(!window->state.shouldClose)
    {
        float deltaTime = getDeltaTime();
        app.OnUpdate(deltaTime, window);
    }

    app.Delete();

       
    deleteWindow(window);
    return 0;
}



#endif // __ENTRYPOINT_H_