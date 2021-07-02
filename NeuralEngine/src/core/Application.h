#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Layer.h"
#include "window.h"
#include "src/utils/types.h"

#define APPLICATION_NAME_LENGTH 128

extern void NewApplication(char *ApplicationName);

extern void ApplicationCreateWindow(int width, int height, char* title);

extern void ApplicationTerminate();

extern void ApplicationPushLayer(Layer* layer);

extern const Window* ApplicationGetWindow();

extern void ApplicationLoop();

extern bool ApplicationEventAfterGUI(const Event* event);

#endif // __APPLICATION_H_