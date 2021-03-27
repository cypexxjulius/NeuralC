
#include "Layer.h"
#include "src/platform/memory.h"

Layer* newLayer
(   
    Window*(*InitFunc)(), 
    void (*OnUpdateFunc)(float deltaTime, Window* window), 
    bool (*OnEventFunc)(Event* event), 
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