#ifndef __LAYER_H_
#define __LAYER_H_

#include "window.h"

typedef struct Layer
{
    void (*Init)();
    void (*OnUpdate)(float deltaTime,const Window* window);
    bool (*OnEvent)(const Event* event);
    void (*Delete)();
} Layer;

Layer* NewLayer
(   
    void (*InitFunc)(), 
    void (*OnUpdateFunc)(float deltaTime,const Window* window), 
    bool (*OnEventFunc)(const Event* event), 
    void (*DeleteFunc)()
);

#endif // __LAYER_H_