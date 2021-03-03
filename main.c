
#define GLEW_STATIC

#include <cglm/cglm.h>

#include <stdio.h>
#include <string.h>

#include "src/core/window.h"
#include "src/core/system.h"
#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/events/event.h"

#include "src/renderer/renderer.h"

#include "src/core/opengl.h"
#include "src/shader/shader.h"


#include "src/events/keycode.h"
#include "src/platform/memory.h"
#include "src/renderer/camera/OrthoCamera.h"

int main()
{
    n_Window *window = n_createWindow(1200, 800, "Test Window");

    float positions[] = {
        -0.5, -0.5, 0.0f,
         0.5, -0.5, 0.0f,
         0.5,  0.5, 0.0f,
        -0.5,  0.5, 0.0f

    };
    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };


    n_VertexArray* vertexArray = newVertexArray();


    n_VertexBuffer vertexBuffer = newVertexBuffer(positions, sizeof(positions));

    n_VertexBufferLayout* layout = newVertexBufferLayout();

    vertexBufferLayoutPush(layout, GL_FLOAT, 3);

    vertexArrayAddBuffer(vertexArray, &vertexBuffer, layout);



    n_IndexBuffer* indexBuffer = newIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    newShader(window, "res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");
    
    shaderBind(window);

    

    shaderUnbind();
    vertexBufferUnbind();
    indexBufferUnbind();

    OrthographicCamera* cam = newOrthographicCamera(-0.9, 0.9, 0.6, -0.6);


    float speed = 0.1f;


    while(!window->shouldClose)
    {


        vec2s pos = cam->position;

        if(isButtonPressed(window, NL_KEY_ESCAPE))
            window->shouldClose = 1;


        if(isButtonPressed(window, NL_KEY_W))
            pos.y -= speed;
        
        else if(isButtonPressed(window, NL_KEY_S))
            pos.y += speed;
        
        if(isButtonPressed(window, NL_KEY_A))
            pos.x += speed;
        
        else if(isButtonPressed(window, NL_KEY_D))
            pos.x -= speed;

        orthographicCameraSetPosition(cam, pos);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f,1.0f, 1.0f,1.0f);

        shaderBind(window);


        shaderUploadUniform1m4(window, "u_viewProj", orthographicCameraGetViewPosMat(cam));
        
        
        vertexArrayBind(vertexArray);
        indexBufferBind(indexBuffer);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // Swap Buffers 
        glfwSwapBuffers(window->windowHandle);
        glfwPollEvents();

    }
    
    deleteIndexBuffer(indexBuffer);
    deleteVertexBuffer(vertexBuffer);
    deleteVertexBufferLayout(layout);
    deleteVertexArray(vertexArray);
    deleteOrthographicCamera(cam);

    deleteWindow(window);

    printf("Memory Count : %i \n", getMemoryCount());

    return 0;
}
