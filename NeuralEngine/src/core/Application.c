#include "src/platform/memory.h"
#include "error.h"
#include "Application.h"

static Application* App = 0;

Application *newApplication(char *ApplicationName)
{
    Application* this = CreateObject(Application);

    u32 NameLength = sizeof(ApplicationName) + 1;
    this->name = MemAlloc(NameLength);
    MemCpy(this->name, ApplicationName, NameLength);


    return this;
}

void SetApplication(Application* app)
{   
    Assert(App != 0, "Redefinition of the Application");

    App = app; 
}   