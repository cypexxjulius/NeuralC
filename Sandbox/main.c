#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/utils/Timer.h"

static CameraController* camera = NULL;

static Texture2D* texture = NULL;
static Texture2D* texture2 = NULL;



void NeuralInit()
{
    unsigned int width = 1280, height = 720;
    ApplicationCreateWindow(width, height, "Test Window"); 

    camera = NewOrthographicCameraController((float)width / (float)height, KeyboardController | MouseDragController);

    texture = NewTexture2D("res/textures/Checkerboard.png");
    texture2 = NewTexture2D("res/textures/firstImage.jpg");

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
                .position = v2(0.0f, 0.0f), 
                .color = v4(0.3f, 1.0f, 1.0f, 1.0f)

            }); 
            
            for(float i = -5; i < 500; i += 0.5)
            {
                for(float k = -5; k < 50; k += 0.5)
                {
                    Renderer2DDrawQuad((Quad2D){ 
                        .position = v2(i, k), 
                        .scale = v2(0.45f, 0.45f), 
                        .color = v4((i + 5) / 10.0f, 0.4f, (k + 5) / 10.0f, 0.5f)
                    });
                }
            }


            // Background
            
            
            Renderer2DDrawQuad((Quad2D){
                .position = v2(-5.0f, -5.0f),
                .scale = v2(2, 2),
                .texture= texture2,
            });
            
            
            Renderer2DDrawQuad((Quad2D){ 
                .position = v2(-50.0f, -50.0f), 
                .scale = v2(100.0f, 100.0f), 
                .color = v4(1.0f, 1.0f, 0.0f, 0.2f),
                .texture= texture,
                .tiling = 10.0f
            });
            

            Renderer2DEndScene();

            Renderer2DBeginScene(NULL);
            {
                
                Renderer2DDrawQuad((Quad2D){
                    .position = v2(-0.95f, 0.0f),
                    .scale = v2(0.1f, 0.1f),
                    .zIndex = 0.5,
                    .color = v4(0.0f, 0.0f, 1.0f, 1.0f)
                });
                
                Renderer2DDrawQuad((Quad2D){
                    .position = v2(-1.0f, 0.0f),
                    .zIndex = 1,
                    .scale = v2(0.1f, 0.5f),
                    .color = v4(0.0f, 1.0f, 1.0f, 1.0f),
                });
            }
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
    DeleteTexture2D(texture2);
    DeleteOrthographicCameraController(camera);
}

void CreateApplication()
{
    NewApplication("Test Game");
    ApplicationPushLayer(NewLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete));
}