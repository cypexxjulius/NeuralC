#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Layer.h"
#include "window.h"
#include "src/utils/types.h"

#define APPLICATION_NAME_LENGTH 128

typedef struct Application
{   
    Window* window;
    Vector layerStack;

    float deltaTime;
    char name[APPLICATION_NAME_LENGTH];

    unsigned int shouldClose;
    volatile u8 minimized;
} Application;

extern void NewApplication(char *ApplicationName);

extern void ApplicationCreateWindow(int width, int height, char* title);

extern void ApplicationTerminate();

extern void ApplicationPushLayer(Layer* layer);

extern const Window* ApplicationGetWindow();

extern void ApplicationLoop();

extern v2 InputGetMousePosition();
extern int InputIsButtonPressed(int key);
extern void SetMouseGrabbed(unsigned int grabbed);
extern int InputIsMouseButtonPressed(int key);
extern v2 GetWindowSize();

#endif // __APPLICATION_H_