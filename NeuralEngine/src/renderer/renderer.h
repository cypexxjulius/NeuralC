#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "Buffer.h"
#include "vertexArray.h"
#include "timestep.h"
#include "shader/shader.h"
#include "camera/Camera.h"
#include "src/utils/types.h"
#include "src/events/eventStructs.h"
#include <cglm/mat4.h>

extern void RendererShutdown();

extern void RendererInit();

extern void RendererClearScreen();

extern void RendererDrawIndexed(VertexArray* va, u32 indexCount);

extern void RendererSetViewPort(unsigned int width, unsigned int height);

extern void RendererStartCallback();

extern void RendererEndCallback();


#endif // __RENDERER_H_