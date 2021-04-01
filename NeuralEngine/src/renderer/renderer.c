#include "renderer.h"


#include <glad/glad.h>


#include "src/core/window.h"
#include "src/core/error.h"
#include "src/core/Application.h"



static Camera* Cam = NULL;
static const Window* window = NULL;

extern void RendererBeginScene(Camera* cam)
{       
    Cam = cam;

    if(!window) 
        window = ApplicationGetWindow();
}

extern void RendererClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f,1.0f, 1.0f,1.0f);
}


extern void RendererSubmit
(VertexArray* va, IndexBuffer* ib, Shader* shader, mat4s transform)
{        

    Assert(!Cam, "Camera must be defined with RendererBeginScene before Calling RendererSubmit"); 

    vertexArrayBind(va);
    indexBufferBind(ib);
    shaderBind(shader);

    // Upload cameraViewPosMat to shader
    shaderUploadUniform1m4(shader, "u_viewProj", orthographicCameraGetViewPosMat(Cam).raw);

    // Upload Transformationmatrix to shader
    shaderUploadUniform1m4(shader, "u_Transform", transform.raw);


    // Draw Elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void RendererEndScene()
{
    Cam = NULL;
    
    // Swap Buffers 
    glfwSwapBuffers(window->windowHandle);
    glfwPollEvents();
}