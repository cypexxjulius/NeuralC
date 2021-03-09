#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "buffer.h"
#include "vertexArray.h"
#include "timestep.h"
#include "shader/shader.h"
#include "camera/Camera.h"
#include "textures/texture.h"

struct n_Window;

extern void rendererDraw
(struct n_Window* window, n_VertexArray* va, n_IndexBuffer* ib, n_Shader shader, n_Camera *cam);

extern void rendererClearScreen();

extern void rendererSwapBuffers(struct n_Window* window);

#endif // __RENDERER_H_