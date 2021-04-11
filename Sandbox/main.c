#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/renderer/shader/shader.h"
#include "src/platform/memory.h"

static CameraController* camera = NULL;

static Texture2D* texture = NULL;

void NeuralInit()
{
    unsigned int width = 1280, height = 720;
    ApplicationCreateWindow(width, height, "Test Window"); 

    camera = NewOrthographicCameraController((float)width / (float)height, KeyboardController | MouseDragController);

    texture = NewTexture2D("res/textures/Checkerboard.png");
}

void NeuralOnUpdate(float deltaTime, const Window* window)
{
    CameraControllerOnUpdate(camera, deltaTime);    

    
    RendererClearScreen();

    Renderer2DBeginScene(camera->camera);

    Renderer2DDrawColoredQuad(v3(0.0f, 0.0f, 0.0f), v2(0.8f, 0.8f), v4(0.3f, 1.0f, 1.0f, 1.0f));
    Renderer2DDrawColoredQuad(v3(2.0f, 0.3f, 0.0f), v2(0.5f, 0.5f), v4(0.8f, 0.2f, 0.8f, 1.0f));
    Renderer2DDrawTexturedQuad(v3(0.0f, 0.0f, -0.1f), v2(100.0f, 100.0f), texture);

    Renderer2DEndScene();
    
}


bool NeuralOnEvent(const Event* event)
{

    CameraControllerOnEvent(camera, event);
    
    switch(event->type)
    {

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
    }
    return 0;
}


void NeuralDelete()
{
    DeleteTexture2D(texture);
    DeleteOrthographicCameraController(camera);
}

Layer* SetGameLayer()
{
    return NewLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete);
}