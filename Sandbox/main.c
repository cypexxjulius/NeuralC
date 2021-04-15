#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/utils/Timer.h"

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
    

    Profile(NeuralOnUpdate)
    {
        Profile(CameraControllerOnUpdate)
        {
            CameraControllerOnUpdate(camera, deltaTime);    
        }

        Profile(RendererPrep)
        {
            RendererClearScreen();
        }

        Profile(RendererDraw)
        {
            Renderer2DBeginScene(camera->camera);

            Renderer2DDrawQuad((Quad2D){ 
                .position = v3(0.0f, 0.0f, 0.0f), 
                .color = v4(0.3f, 1.0f, 1.0f, 1.0f) 
            }); 
            
            Renderer2DDrawQuad((Quad2D){ 
                .position = v3(2.0f, 0.3f, 0.0f), 
                .scale = v2(0.5f, 0.5f), 
                .color = v4(0.8f, 0.2f, 0.0f, 1.0f)
            });

            Renderer2DDrawQuad((Quad2D){ 
                .position = v3(0.0f, 0.0f, -0.1f), 
                .scale = v2(100.0f, 100.0f), 
                .texture= texture,
                .color = v4(1.0f, 1.0f, 1.0f, 1.0f),
                .tilling = 10.0f,
                .rotation = 45.0f
            });

            Renderer2DEndScene();
        }
    }
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