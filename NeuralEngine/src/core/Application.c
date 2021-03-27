#include "src/platform/memory.h"
#include "Application.h"

static Application* App;

Application *newApplication(char *ApplicationName)
{
    Application* this = CreateObject(Application);

    u32 NameLength = sizeof(ApplicationName) + 1;
    this->name = MemAlloc(NameLength);
    MemCpy(this->name, ApplicationName, NameLength);


    return this;
}

int main()
{   
    App = newApplication("Test Layer");

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
    
       
    deleteWindow(window);
    return 0;
}

