#pragma once

#include "buffer.h"
#include "vertexArray.h"
#include "timestep.h"
#include "shader/shader.h"
#include "camera/Camera.h"
#include "textures/texture.h"
#include "src/utils/types.h"
#include <cglm/mat4.h>

extern void RendererBeginScene(Camera* cam);

extern void RendererSubmit
(VertexArray* va, IndexBuffer* ib, Shader* shader, mat4s transform);

extern void RendererClearScreen();

extern void RendererEndScene();

#define NO_TRANSFORM GLMS_MAT4_IDENTITY
