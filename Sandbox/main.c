#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/utils/Timer.h"
#include "src/utils/fileio.h"

static CameraController* camera = NULL;

static Texture2D* texture = NULL;
static Texture2D* texture2 = NULL;
static Texture2D* CharSet = NULL;

static float Rotation = 0.0f;

#define maxSize 100

static char string[4096] = { 0 };
static char FPSCountBuffer[10] = { 0 };
static unsigned int stringCursor = 0;

void NeuralInit()
{   
    unsigned int width = 1280, height = 720;
    ApplicationCreateWindow(width, height, "Test Window"); 

    camera = NewOrthographicCameraController(CameraMouseDragController | CameraMouseScrollSensitive);

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
            
            // Checkerboard Background 
            Renderer2DDrawQuad( (Quad2D) {
                .position = V2(0.0f, 0.0f), 
                .width = 50.0f,
                .height = 50.0f,
                .texture = texture,
                .rotation = Rotation,
            });

            Rotation += deltaTime;
            
            
            Renderer2DDrawQuad( (Quad2D) {
                .position = V2(0.0f, 10.0f), 
                .tiling = 10.0f,
                .width = 50.0f,
                .height = 50.0f,
                .color = v4(1.0, 0.3, 0.1, 1.0),
            });

            Renderer2DDrawQuad((Quad2D) {
                .position = V2(-1.0f, -0.5f),
                .color = v4(0.0f, 1.0f, 1.0f, 1.0f),
                .width = 1000 nu,
                .height = 200 nu,
                .text = (TextElement []) {
                    (TextElement) {
                        .string = string,
                        .color = v3(1.0, 1.0, 1.0)
                    }
                }
            });
            
            Renderer2DEndScene();


            static char buffer[100];
            static float TimeBuffer = 0;
            if(TimeBuffer > 0.05){
                snprintf(buffer, 100, "FrameTime : %fms", deltaTime * 1000,3);
                TimeBuffer = 0;
            }else {
                TimeBuffer += deltaTime;
            }


            Renderer2DBeginScene(NULL);
            {

                Renderer2DDrawQuad((Quad2D) {
                    .position = V2(0.0, 0.0),
                    .texture = texture,
                    .width = 50 nu,
                    .height = 50 nu,
                });

                Renderer2DDrawQuad((Quad2D) {
                    .position = V2(-1.75f, 0.0f),
                    .color = v4(1.0, 1.0, 1.0, 0.1),
                    .width = 1000 nu,
                    .height = 200 nu,
                    .text = (TextElement []) {
                        (TextElement) {
                            .string = buffer,
                            .color = v3(1.0, 1.0, 1.0)
                        }
                    }
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
                    ApplicationTerminate();
                    break;
                case NL_KEY_BACKSPACE:

                    if(stringCursor == 0 || event->KeyPressedEvent.action == 0)
                        return true;

                    if(event->KeyPressedEvent.mod == NL_SHIFT_MOD)
                    {
                        string[0] = 0;
                        stringCursor = 0;
                        return true;
                    }

                    string[--stringCursor] = 0;
                    break;    
                case NL_KEY_ENTER:
                    string[stringCursor++] = '\n';   
            }   

        }break;

        case(CharEventType) :
        {
            
            char keycode = (char)event->KeyPressedEvent.keyCode;

            string[stringCursor++] = keycode;
            string[stringCursor] = 0;
        }
    }
    return 0;
}


void NeuralDelete()
{
    DeleteTexture2D(texture);
    DeleteTexture2D(texture2);
    DeleteOrthographicCameraController(camera);
}

void Start()
{
    NewApplication("Test Game");
    ApplicationPushLayer(NewLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete));
}