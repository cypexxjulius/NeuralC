
#include "Layer.h"
#include "src/platform/memory.h"

Layer* NewLayer(Layer layer)
{
    Layer* this = CreateObject(Layer);
    *this = layer;

    return this;
}