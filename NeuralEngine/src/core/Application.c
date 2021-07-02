#include "Application.h"

#include "src/platform/memory.h"
#include "error.h"
#include "src/events/event.h"
#include "window.h"
#include "src/utils/types.h"
#include "src/renderer/timestep.h"
#include "src/events/keycode.h"
#include "src/renderer/Renderer.h"
#include "src/gui/GUI.h"
#include "src/utils/Logger.h"

typedef struct Application
{   
    Window* window;
    Vector layerStack;

    float deltaTime;
    char name[APPLICATION_NAME_LENGTH];

    volatile u8 shouldClose;
    volatile u8 minimized;
} Application;

static Application App;

extern void NewApplication(char *ApplicationName)
{
    App = (Application){ 0 };

    u32 NameLength = sizeof(ApplicationName) + 1;
    Assert(NameLength >= APPLICATION_NAME_LENGTH, "ApplicationName too long");
    Memory.Copy(App.name, ApplicationName, NameLength);
    
    NewVector(&App.layerStack, 2, sizeof(Layer *), VECTOR_POINTER | VECTOR_FREE);
}

static void ApplicationOnWindowResizedEvent(const Event* event)
{
    if(event->WindowResizeEvent.width == 0 || event->WindowResizeEvent.width == 0)
    {
        App.minimized = true;
        return;
    }
    
    App.minimized = false;
    RendererSetViewPort(event->WindowResizeEvent.width, event->WindowResizeEvent.height);
}


static void ApplicationOnEvent(Event* event)
{
    if(event->type == WindowCloseEventType)
    {
        App.shouldClose = 1;
        return;
    }

    if(event->type == WindowResizeEventType)
        ApplicationOnWindowResizedEvent(event);

    GUIOnEvent(event);
}

bool ApplicationEventAfterGUI(const Event* event)
{
    Layer* layer;
    for(unsigned int i = 0; i < App.layerStack.used; i++)
    {       
        layer = VectorGet(&App.layerStack, i);
        if(layer->OnEvent == NULL)
            continue;

        if(layer->OnEvent(event) && event->Cancable)
            return true;
    }

    return false;
}


void ApplicationCreateWindow(int width, int height, char* title)
{
    App.window = NewWindow(width, height, title);
    App.window->EventCallback = ApplicationOnEvent;
    InitEventSystem(App.window);
    InitError();
    RendererInit();
    InitGUI();
}

void ApplicationLoop()
{
    Assert(!App.window, "ApplicationCreateWindow must be called before leaving the Init function");


    Layer* activeLayer = NULL;
    while(!App.shouldClose)
    {
        int MemoryCount = GetMemoryCount();

        if(App.minimized == true)
        {
            WindowPollEvents();
            continue;
        }

        App.deltaTime = GetDeltaTime();
        RendererStartCallback();

        GUIBegin(App.deltaTime);

        for(unsigned int i = 0; i < App.layerStack.used; i++)
        {
            activeLayer = VectorGet(&App.layerStack, i);

            if(activeLayer->OnUpdate != NULL)
                activeLayer->OnUpdate(App.deltaTime);
        }

        GUIEnd();
        RendererEndCallback();

        WindowSwapBuffers(App.window);
        WindowPollEvents();

        if((int)GetMemoryCount() - (int)MemoryCount > 0)
            LOG("[WARNING] Allocation Loss %i", (int)GetMemoryCount() - (int)MemoryCount);
    }

    // Cleanup
    for(unsigned int i = 0; i < App.layerStack.used; i++)
    {
        activeLayer = VectorGet(&App.layerStack, i);
        activeLayer->Delete();
    }
    
    // Shutdown Renderer
    RendererShutdown();
    DeinitGUI();

    DeleteVector(&App.layerStack);
    DeleteWindow(App.window);
}

void ApplicationPushLayer(Layer* layer)
{
    VectorAdd(&App.layerStack, layer);
    
    if(layer->Init != NULL)
        layer->Init();
}

const Window* ApplicationGetWindow()
{
    Assert(!App.window, "Window object does not exist");
    return App.window;
}

void ApplicationTerminate()
{
    Assert(!App.window, "Window object does not exist");
    App.shouldClose = 1;
}

v2  InputGetMousePosition()
{
    
    Assert(!App.window, "Window object does not exist");
    return App.window->state.mouse.position;
}

int  InputIsButtonPressed(int key)
{
    
    Assert(!App.window, "Window object does not exist");
    return App.window->state.keyboard.keys[key].down;
}

int InputIsMouseButtonPressed(int key)
{
    Assert(!App.window, "Window object does not exist");
    return App.window->state.mouse.buttons[key].down;
}

void SetMouseGrabbed(unsigned int grabbed) 
{
    
    Assert(!App.window, "Window object does not exist");
    glfwSetInputMode(App.window->windowHandle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

v2 GetWindowSize()
{
    Assert(!App.window, "Window object does not exist");
    return V2((float)App.window->state.width, (float)App.window->state.height);
}