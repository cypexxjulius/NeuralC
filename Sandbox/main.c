#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/renderer/shader/shader.h"
#include "src/platform/memory.h"

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

VertexArray* vertexArray;

VertexBuffer vertexBuffer;


IndexBuffer* indexBuffer;

Camera* cam;

Texture* texture;

ShaderLibrary* shaderLibrary;


float speed = 5.0f;

v2 delta = {0};

v2 DisplayRatio = {0};

float scaleFactor = 0.1f;

void NeuralInit()
{
    ApplicationCreateWindow(1280, 720, "Test Window"); 

    vertexArray = NewVertexArray();
    
    vertexBuffer = NewVertexBuffer(positions, sizeof(positions));
    

    VertexBufferLayout* layout = NewVertexBufferLayout(
        2, 
        BufferElement(2, GL_FLOAT),
        BufferElement(2, GL_FLOAT)
    );
    
    vertexArrayAddBuffer(vertexArray, &vertexBuffer, layout);

    DeleteVertexBufferLayout(layout);
    
    indexBuffer = NewIndexBuffer(indices, ArraySize(indices, unsigned int));
    
    shaderLibrary = NewShaderLibrary(3);

    Shader *flatColorShader = ShaderLibraryLoadShader(shaderLibrary, "FlatColorShader", "res/shader/FlatColor.glsl");
    
    shaderBind(flatColorShader);

    shaderUploadUniform4f(flatColorShader, "u_Color", 1.0, 0.3, 0.8, 1.0f);
    
    Shader * shader = ShaderLibraryLoadShader(shaderLibrary, "TextureShader", "res/shader/TextureShader.glsl");

    cam = NewOrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
    
    
    
    texture = NewTexture("res/textures/Placeholder.jpg");


    shaderBind(shader);
    textureBind(texture, 0);

    
    shaderUploadUniform1i(shader, "u_Texture", 0);
    
}

void NeuralOnUpdate(float deltaTime, const Window* window)
{
    v2 pos = orthographicCameraGetPosition(cam);
    
    
    if(InputIsButtonPressed(NL_KEY_W))
        pos.y -= speed * deltaTime;
    
    
    else if(InputIsButtonPressed(NL_KEY_S))
        pos.y += speed * deltaTime;
    
    
    if(InputIsButtonPressed(NL_KEY_A))
        pos.x += speed * deltaTime;
    
    else if(InputIsButtonPressed(NL_KEY_D))
        pos.x -= speed * deltaTime;
    
    pos.x += delta.x * (cam->orthoCam.height / window->state.height);
    pos.y -= delta.y * (cam->orthoCam.width  / window->state.width);

    delta = v2(0.0f, 0.0f);
    orthographicCameraSetPosition(cam, pos);

    
    
    Shader* shader = ShaderLibraryGetShader(shaderLibrary, "TextureShader");
    
    mat4s scale = glms_scale_make(vec3s(scaleFactor, scaleFactor, 0.0f ));

    RendererClearScreen();

    RendererBeginScene(cam);
    {
        
        for(int i = 0; i < 10; i++)
        {
            for(int k = 0; k < 10; k++)
            { 
                mat4s transform_temp    = glms_translate_make(vec3s( i * scaleFactor * 1.1f, k * scaleFactor * 1.1f, 0.0f));
                mat4s transform         = glms_mat4_mul(transform_temp, scale);
                RendererSubmit(vertexArray, indexBuffer, shader, transform);
            }
        }
        shader = ShaderLibraryGetShader(shaderLibrary, "FlatColorShader");
        RendererSubmit(vertexArray, indexBuffer,shader, NO_TRANSFORM);
    }
    RendererEndScene();
}


bool NeuralOnEvent(const Event* event)
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
            switch(event->KeyPressedEvent.keyCode)
            {
                case NL_MOUSE_BUTTON_LEFT :
                {
                    isPressed = event->KeyPressedEvent.action != 0; 
                } break;
            }
            
        }break;

        case (KeyPressedEventType) :
        {
            switch (event->KeyPressedEvent.keyCode)
            {
                case NL_KEY_ESCAPE :
                {
                    ApplicationTerminate();
                }break;
            }
        }break;

        case(ScrolledEventType) : 
        {
            if(event->PosEvent.pos.y == 0)
                break;
            
            float scrollIntensity = 2.0f;

            if(event->PosEvent.pos.y > 0)
            {
                scaleFactor *= event->PosEvent.pos.y * scrollIntensity;
            }
            else 
            {
                scaleFactor /= -event->PosEvent.pos.y * scrollIntensity;
            }
        }
    }
    return 0;
}


void NeuralDelete()
{
    DeleteIndexBuffer(indexBuffer);
    DeleteVertexBuffer(vertexBuffer);
    DeleteVertexArray(vertexArray);
    DeleteOrthographicCamera(cam);
    DeleteShaderLibrary(shaderLibrary);
    DeleteTexture(texture);
}

Layer* SetGameLayer()
{
    return NewLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete);
}