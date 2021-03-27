#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "buffer.h"
#include "vertexArray.h"
#include "timestep.h"
#include "shader/shader.h"
#include "camera/Camera.h"
#include "textures/texture.h"

extern void RendererBeginScene(Camera* cam);

extern void RendererSubmit
(n_VertexArray* va, n_IndexBuffer* ib, n_Shader shader);

extern void RendererClearScreen();

extern void RendererEndScene();

#endif // __RENDERER_H_