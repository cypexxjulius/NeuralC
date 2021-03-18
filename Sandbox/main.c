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

Camera* cam;

n_Texture* texture;


float speed = 5.0f;

static Window* LocalWindow = NULL; 

v2 delta = {0};

Window* NeuralInit()
{
    Window *window = CreateWindow(1200, 800, "Test Window"); 
    LocalWindow = window;
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

void NeuralOnUpdate(float deltaTime, Window* window)
{
    v2 pos = orthographicCameraGetPosition(cam);

    if(IsButtonPressed(window, NL_KEY_ESCAPE))
        window->state.shouldClose = 1;
        
        
    if(IsButtonPressed(window, NL_KEY_W))
        pos.y -= speed * deltaTime;
        

    else if(IsButtonPressed(window, NL_KEY_S))
        pos.y += speed * deltaTime;
        
    if(IsButtonPressed(window, NL_KEY_A))
        pos.x += speed * deltaTime;
        
    else if(IsButtonPressed(window, NL_KEY_D))
        pos.x -= speed * deltaTime;
    
    
    pos.x += delta.x * (cam->orthoCam.width / window->state.width);
    pos.y -= delta.y * (cam->orthoCam.height / window->state.height);
    delta = v2(0, 0);

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

bool NeuralOnEvent(Event* event)
{
    static u8 isPressed = 0;

    switch(event->type)
    {
        case(MouseMovedEventType) :
        {
            if(isPressed)
            {
                delta.x += event->PosEvent.delta.x;
                delta.y += event->PosEvent.delta.y;
            }
        }break;

        case(MouseButtonPressedEventType) :
        {
            if(event->KeyPressedEvent.keyCode == NL_MOUSE_BUTTON_LEFT)
            {
                isPressed = event->KeyPressedEvent.action;
            } 
        }break;

    }
    return 0;
}

Application CreateApplication()
{
    return Application(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete);
}