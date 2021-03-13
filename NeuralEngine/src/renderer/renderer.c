#include "renderer.h"
#include <glad/glad.h>
#include "src/core/window.h"
#include "src/core/error.h"

extern void rendererClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f,1.0f, 1.0f,1.0f);
}

extern void rendererSwapBuffers(struct Window* window)
{
    // Swap Buffers 
    glfwSwapBuffers(window->windowHandle);
    glfwPollEvents();
}


extern void rendererDraw
(struct Window* window, n_VertexArray* va, n_IndexBuffer* ib, n_Shader shader, Camera *cam)
{            
    vertexArrayBind(va);
    indexBufferBind(ib);
    shaderBind(shader);

    if(cam != NULL)
    {

    // Upload cameraViewPosMat to shader
    if (cam->camType == OrthographicCameraType)
        shaderUploadUniform1m4(shader, "u_viewProj", orthographicCameraGetViewPosMat(cam));

    else 
        ASSERT(0, "Wrong Camera type")
    }

    // Draw Elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}