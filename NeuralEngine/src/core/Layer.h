#ifndef __LAYER_H_
#define __LAYER_H_

#include "window.h"

typedef struct Layer
{
    Window* (*Init)();
    void (*OnUpdate)(float deltaTime, Window* window);
    bool (*OnEvent)(Event* event);
    void (*Delete)();
} Layer;

Layer* NewLayer
(   
    Window*(*InitFunc)(), 
    void (*OnUpdateFunc)(float deltaTime, Window* window), 
    bool (*OnEventFunc)(Event* event), 
    void (*DeleteFunc)()
);


#endif //__LAYER_H_