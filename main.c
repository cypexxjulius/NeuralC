#include <stdio.h>
#include <string.h>

#include "src/core/window.h"
#include "src/renderer/renderer.h"
#include "src/events/keycode.h"

#include "src/platform/memory.h"

int main()
{
    n_Window *window = n_createWindow(1200, 800, "Test Window");

    float positions[] = {
        -0.5, -0.5, 0.0f, 0.0f,
         0.5, -0.5, 1.0f, 0.0f,
         0.5,  0.5, 1.0f, 1.0f,
        -0.5,  0.5, 0.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };


    n_VertexArray* vertexArray = newVertexArray();


    n_VertexBuffer vertexBuffer = newVertexBuffer(positions, sizeof(positions));

    n_VertexBufferLayout* layout = newVertexBufferLayout();
    vertexBufferLayoutPush(layout, GL_FLOAT, 2);
    vertexBufferLayoutPush(layout, GL_FLOAT, 2);

    vertexArrayAddBuffer(vertexArray, &vertexBuffer, layout);



    n_IndexBuffer* indexBuffer = newIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    n_Shader shader = newShader("res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");
    shaderBind(shader);

    n_Camera* cam = newOrthographicCamera(-0.9, 0.9, 0.6, -0.6);

    
    n_Texture* texture = newTexture("res/textures/firstImage.jpg");
    textureBind(texture, 0);
    shaderUploadUniform1i(shader, "u_Texture", 0);
    

    float speed = 5.0f;
    
    

    while(!window->shouldClose)
    {
        float deltaTime = getDeltaTime();


        v2 pos = orthographicCameraGetPosition(cam);

        if(isButtonPressed(window, NL_KEY_ESCAPE))
            window->shouldClose = 1;
        
        
        if(isButtonPressed(window, NL_KEY_W))
            pos.y -= speed * deltaTime;
        

        else if(isButtonPressed(window, NL_KEY_S))
            pos.y += speed * deltaTime;
        
        if(isButtonPressed(window, NL_KEY_A))
            pos.x += speed * deltaTime;
        
        else if(isButtonPressed(window, NL_KEY_D))
            pos.x -= speed * deltaTime;

        orthographicCameraSetPosition(cam, pos);


        rendererClearScreen();
        
        rendererDraw(window, vertexArray, indexBuffer, shader, cam);

        rendererSwapBuffers(window);
    }

    deleteIndexBuffer(indexBuffer);
    deleteVertexBuffer(vertexBuffer);
    deleteVertexBufferLayout(layout);
    deleteVertexArray(vertexArray);
    deleteOrthographicCamera(cam);
    deleteShader(shader);
    deleteTexture(texture);
    
    deleteWindow(window);
    return 0;
}
