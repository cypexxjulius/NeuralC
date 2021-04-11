#include "Application.h"

#include "src/platform/memory.h"
#include "error.h"
#include "src/events/event.h"
#include "window.h"
#include "src/utils/types.h"
#include "src/renderer/timestep.h"
#include "src/events/keycode.h"
#include "src/renderer/renderer.h"

static Application App = { 0 };

extern void CreateApplication(char *ApplicationName)
{
    u32 NameLength = sizeof(ApplicationName) + 1;
    App.name = MemAlloc(NameLength);
    MemCpy(App.name, ApplicationName, NameLength);
    
    App.layerStack = NewVector(2, sizeof(Layer *), VECTOR_POINTER | VECTOR_FREE);
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
    {
        ApplicationOnWindowResizedEvent(event);
    }

    Layer* layer;
    for(unsigned int i = 0; i < App.layerStack->used; i++)
    {       
        layer = VectorGet(App.layerStack, i);
        if(layer->OnEvent(event))
            break;
    }
}


extern void ApplicationCreateWindow(int width, int height, char* title)
{
    App.window = NewWindow(width, height, title);
    App.window->EventCallback = ApplicationOnEvent;
    InitEventSystem(App.window);
    InitError();
    RendererInit();
}

extern void ApplicationLoop()
{
    Assert(!App.window, "Window does not exist");
    Layer* activeLayer = NULL;
    while(!App.shouldClose)
    {
        if(App.minimized == false)
        {
            for(unsigned int i = 0; i < VectorLength(App.layerStack); i++)
            {
                activeLayer = VectorGet(App.layerStack, i);
                activeLayer->OnUpdate(GetDeltaTime(), App.window);
            }
            WindowUpdate(App.window, !App.minimized);
        }
    }

    // Cleanup
    for(unsigned int i = 0; i < VectorLength(App.layerStack); i++)
    {
        activeLayer = VectorGet(App.layerStack, i);
        activeLayer->Delete();
    }

    MemFree(App.name);
    
    // Shutdown Renderer
    RendererShutdown();


    DeleteVector(App.layerStack);
    DeleteWindow(App.window);
}

extern void ApplicationLayerAdd(Layer* layer)
{
    VectorAdd(App.layerStack, layer);
    layer->Init();
}

const Window* ApplicationGetWindow()
{
    Assert(!App.window, "Window object does not exist");
    return App.window;
}

void ApplicationTerminate()
{
    App.shouldClose = 1;
}

extern v2  InputGetMousePosition()
{
    
    Assert(!App.window, "Window object does not exist");
    return App.window->state.mouse.position;
}

extern int  InputIsButtonPressed(int key)
{
    
    Assert(!App.window, "Window object does not exist");
    return App.window->state.keyboard.keys[key].down;
}

extern int InputIsMouseButtonPressed(int key)
{
    Assert(!App.window, "Window object does not exist");
    return App.window->state.mouse.buttons[key].down;
}

extern void SetMouseGrabbed(unsigned int grabbed) 
{
    
    Assert(!App.window, "Window object does not exist");
    glfwSetInputMode(App.window->windowHandle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}