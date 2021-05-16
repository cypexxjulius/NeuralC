#include "renderer.h"


#include <glad/glad.h>

#include "src/core/debug.h"
#include "src/core/window.h"
#include "src/core/error.h"
#include "src/core/Application.h"
#include "Renderer2D.h"

#include "src/utils/types.h"
#include <stdio.h>

static mat4s viewProjMat = { 0 };
static const Window* window = NULL;


static void APIENTRY OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if(severity <= GL_DEBUG_SEVERITY_MEDIUM)
        return;


    (void)source;
    (void)length;
    (void)userParam;
    (void)id;
    
    CoreWarn("[OPENGL ERROR]:\n" 
            "Source     : 0x%x\n"
            "Type       : 0x%x\n"
            "Severity   : 0x%x\n"
            "ERROR:\n%s\n", source, type, severity, message);
}


extern void RendererInit()
{

#ifdef NEURAL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLErrorCallback, NULL);
		
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    Renderer2DInit();
}

void RendererShutdown()
{
    Renderer2DShutdown();
}

void RendererClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f,0.2f, 0.1f,1.0f);
}


void RendererDrawIndexed(VertexArray* va, u32 indexCount)
{
    u32 count = indexCount ? indexCount : va->indexBuffer->count;
    // Draw Elements
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
}

void RendererSetViewPort(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);
}

void RendererEndCallback()
{
    Renderer2DEndSceneCallback();
}

void RendererStartCallback()
{
    Renderer2DStartSceneCallback();
}
