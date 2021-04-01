
#include "Layer.h"
#include "src/platform/memory.h"

Layer* NewLayer
(   
    void (*InitFunc)(), 
    void (*OnUpdateFunc)(float deltaTime, const Window* window), 
    bool (*OnEventFunc)(const Event* event), 
    void (*DeleteFunc)()
)
{
    Layer* this = CreateObject(Layer);
    this->Init = InitFunc;
    this->OnEvent = OnEventFunc;
    this->OnUpdate = OnUpdateFunc;
    this->Delete = DeleteFunc;

    return this;
}