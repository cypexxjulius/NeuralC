#ifndef __ENTRYPOINT_H_
#define  __ENTRYPOINT_H_

#include "Application.h"
#include "error.h"
#include "src/events/event.h"
#include "src/renderer/timestep.h"


Layer* SetGameLayer();


int main()
{   
    Application* App = NewApplication("Test Layer");

    App->gameLayer = SetGameLayer();    

    Window* window = App->gameLayer->Init();
    window->ErrorCallback = App->gameLayer->OnEvent;

    InitError();

    InitEventSystem(window);

    while(!window->state.shouldClose)
    {
        float deltaTime = getDeltaTime();
        App->gameLayer->OnUpdate(deltaTime, window);
    }

    App->gameLayer->Delete();
    
       
    DeleteWindow(window);
    return 0;
}




#endif // __ENTRYPOINT_H_