#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "buffer.h"
#include "vertexArray.h"
#include "timestep.h"
#include "shader/shader.h"
#include "camera/Camera.h"
#include "src/utils/types.h"
#include <cglm/mat4.h>

extern void RendererShutdown();

extern void RendererInit();


extern void RendererBeginScene(Camera* cam);

extern void RendererSubmit
(VertexArray* va, Shader* shader, mat4s transform);

extern void RendererClearScreen();

extern void RendererEndScene();


extern void RendererSetViewPort(unsigned int width, unsigned int height);

void RendererDrawIndexed(VertexArray* va);

#define NO_TRANSFORM GLMS_MAT4_IDENTITY

#endif // __RENDERER_H_