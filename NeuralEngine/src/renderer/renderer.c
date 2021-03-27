#include "renderer.h"


#include <glad/glad.h>


#include "src/core/window.h"
#include "src/core/error.h"



static Camera* Cam = NULL;

extern void RendererBeginScene(Camera* cam)
{       
    Cam = cam;
}

extern void RendererClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f,1.0f, 1.0f,1.0f);
}


extern void RendererSubmit
(n_VertexArray* va, n_IndexBuffer* ib, n_Shader shader)
{        

    ASSERT(Cam, "Camera must be defined with RendererBeginScene before Calling RendererSubmit"); 

    vertexArrayBind(va);
    indexBufferBind(ib);
    shaderBind(shader);

    // Upload cameraViewPosMat to shader
    if (Cam->camType == OrthographicCameraType)
        shaderUploadUniform1m4(shader, "u_viewProj", orthographicCameraGetViewPosMat(Cam));

    else 
        ASSERT(0, "Wrong Camera type")

    // Draw Elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void RendererEndScene()
{
    Cam = NULL;
    // Swap Buffers 
    glfwSwapBuffers(GetWindow()->windowHandle);
    glfwPollEvents();
}