#include <stdio.h>
#include <string.h>

#include <Neural.h>

float positions[] = {
    -0.5, -0.5, 0.0f, 0.0f,
     0.5, -0.5, 1.0f, 0.0f,
     0.5,  0.5, 1.0f, 1.0f,
    -0.5,  0.5, 0.0f, 1.0f,
};
u32 indices[] = {
    0, 1, 2, 
    2, 3, 0
};

n_VertexArray* vertexArray;

n_VertexBuffer vertexBuffer;

n_IndexBuffer* indexBuffer;

n_Shader shader;

n_Camera* cam;

n_Texture* texture;


float speed = 5.0f;

n_Window* NeuralInit()
{
    n_Window *window = n_createWindow(1200, 800, "Test Window"); 
    vertexArray = newVertexArray();
    
    vertexBuffer = newVertexBuffer(positions, sizeof(positions));

    n_VertexBufferLayout* layout = newVertexBufferLayout();
    vertexBufferLayoutPush(layout, GL_FLOAT, 2);
    vertexBufferLayoutPush(layout, GL_FLOAT, 2);

    vertexArrayAddBuffer(vertexArray, &vertexBuffer, layout);

    deleteVertexBufferLayout(layout);

    indexBuffer = newIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    shader = newShader("res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");

    shaderBind(shader);

    cam = newOrthographicCamera(-0.9, 0.9, 0.6, -0.6);

    
    texture = newTexture("res/textures/firstImage.jpg");
    textureBind(texture, 0);
    shaderUploadUniform1i(shader, "u_Texture", 0);
    
    return window;
}

void NeuralOnUpdate(float deltaTime, n_Window* window)
{
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

void NeuralDelete()
{

    deleteIndexBuffer(indexBuffer);
    deleteVertexBuffer(vertexBuffer);
    deleteVertexArray(vertexArray);
    deleteOrthographicCamera(cam);
    deleteShader(shader);
    deleteTexture(texture);
}

bool NeuralOnEvent()
{
    return 0;
}

Application CreateApplication()
{
    return Application(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete);
}