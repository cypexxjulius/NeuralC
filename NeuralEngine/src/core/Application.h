#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Layer.h"
#include "window.h"
#include "src/utils/types.h"

typedef struct Application
{   
    Window* window;
    Vector* layerStack;

    float deltaTime;
    char *name;

    unsigned int shouldClose;

} Application;

void CreateApplication(char* ApplicationName);


void ApplicationCreateWindow(int width, int height, char* title);

void ApplicationTerminate();

void ApplicationLayerAdd(Layer* layer);

const Window* ApplicationGetWindow();

void ApplicationLoop();

extern v2 InputGetMousePosition();
extern int InputIsButtonPressed(int key);
extern void SetMouseGrabbed(unsigned int grabbed);
extern int InputIsMouseButtonPressed(int key);
#endif // __APPLICATION_H_