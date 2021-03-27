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

v2 DisplayRatio = {0};

float scaleFactor = 0.1f;

Window* NeuralInit()
{
    
    LocalWindow = CreateWindow(1280, 720, "Test Window"); 
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
    
    cam = newOrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
    
    
    
    texture = newTexture("res/textures/firstImage.jpg");
    textureBind(texture, 0);
    shaderUploadUniform1i(shader, "u_Texture", 0);
    return LocalWindow;
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
    
    pos.x += delta.x * (cam->orthoCam.height / window->state.height);
    pos.y -= delta.y * (cam->orthoCam.width  / window->state.width);

    delta = v2(0.0f, 0.0f);
    orthographicCameraSetPosition(cam, pos);

    
    mat4s scale = glms_scale_make((vec3s){ scaleFactor, scaleFactor, 0.0f });
    
    RendererClearScreen();

    RendererBeginScene(cam);
    {
        
        for(int i = 0; i < 20; i++)
        {
            for(int k = 0; k < 20; k++)
            { 
                mat4s transform_temp    = glms_translate_make((vec3s){ i * scaleFactor * 1.1f, k * scaleFactor * 1.1f, 0.0f});
                mat4s transform         = glms_mat4_mul(transform_temp, scale);
                RendererSubmit(vertexArray, indexBuffer, shader, transform);
            }
        }
        
        RendererSubmit(vertexArray, indexBuffer, shader, NO_TRANSFORM);
    }
    RendererEndScene(window);
    
}


bool NeuralOnEvent(Event* event)
{
    static u8 isPressed = 0;
    
    switch(event->type)
    {
        case(MouseMovedEventType) :
        {
            delta.x += event->PosEvent.delta.x * isPressed;
            delta.y += event->PosEvent.delta.y * isPressed;
        }break;
        
        case(MouseButtonPressedEventType) :
        {
            if(event->KeyPressedEvent.keyCode == NL_MOUSE_BUTTON_LEFT)
                isPressed = event->KeyPressedEvent.action != 0; 
        }break;

        case(ScrolledEventType) : 
        {
            if(event->PosEvent.pos.y == 0)
                break;
            
            float scrollIntensity = 2.0f;

            if(event->PosEvent.pos.y > 0)
                scaleFactor *= event->PosEvent.pos.y * scrollIntensity;
            else 
                scaleFactor /= -event->PosEvent.pos.y * scrollIntensity;

            printf("ScaleFactor : %f\n",scaleFactor);
        }
    }
    return 0;
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

Layer* SetGameLayer()
{
    return newLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete);
}