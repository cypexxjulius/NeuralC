#ifndef __LAYER_H_
#define __LAYER_H_

#include "window.h"

typedef struct Layer
{
    void (*Init)();
    void (*OnUpdate)(float deltaTime);
    bool (*OnEvent)(const Event* event);
    void (*GUIUpdate)(float deltaTime);
    void (*Delete)();
} Layer;

extern Layer* NewLayer(Layer layer);

#endif // __LAYER_H_