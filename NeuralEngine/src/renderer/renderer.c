#include "renderer.h"


#include <glad/glad.h>


#include "src/core/window.h"
#include "src/core/error.h"
#include "src/core/Application.h"
#include "Renderer2D.h"

#include "src/utils/types.h"
#include <stdio.h>

static mat4s viewProjMat = { 0 };
static const Window* window = NULL;

extern void RendererInit()
{
    Renderer2DInit();
}

extern void RendererShutdown()
{
    Renderer2DShutdown();
}

extern void RendererBeginScene(Camera* cam)
{       
    viewProjMat = orthographicCameraGetViewPosMat(cam);

    if(!window) 
        window = ApplicationGetWindow();
}

extern void RendererClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f,0.1f, 0.1f,1.0f);
}


void RendererDrawIndexed(VertexArray* va)
{
    // Draw Elements
    glDrawElements(GL_TRIANGLES, va->indexBuffer->count, GL_UNSIGNED_INT, NULL);
}

extern void RendererSubmit
(VertexArray* va, Shader* shader, mat4s transform)
{        
    ShaderBind(shader); // Bind Shader

    // Upload cameraViewPosMat to shader
    ShaderSetMat4(shader, "u_viewProj", viewProjMat.raw);

    // Upload Transformationmatrix to shader
    ShaderSetMat4(shader, "u_Transform", transform.raw);


    VertexArrayBind(va);
    RendererDrawIndexed(va);
}

void RendererEndScene()
{
}


extern void RendererSetViewPort(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);
}