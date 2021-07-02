#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/utils/Timer.h"
#include "src/utils/fileio.h"

static CameraController* camera = NULL;

static Texture2D* texture = NULL;
static Texture2D* texture2 = NULL;

static float Rotation = 0.0f;

#define STRING_SIZE 4096
static char string[STRING_SIZE] = { 0 };
static unsigned int stringCursor = 0;
static v2 MousePressedPosition = { 0 };

static unsigned int lastPressedButton = 0;
static u8 showMessage = 0;

void NeuralInit()
{   
    ApplicationCreateWindow(1280, 720, "Test Window"); 

    camera = NewOrthographicCameraController(CameraMouseDragController | CameraMouseScrollSensitive);

    texture = NewTexture2D("res/textures/Checkerboard.png");
    texture2 = NewTexture2D("res/textures/firstImage.jpg");
}

void NeuralOnUpdate(float deltaTime)
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
                GUIBoxBegin("Performance", V2(0, 0));
                    GUIText("FrameTime %.2fms", 1000.0f * deltaTime);
                    GUIText("FPS %.2f", 1.0f / deltaTime);
                    GUIText("Memory Allocated %u", GetMemoryCount());

                    persist bool isUsed = 0;
                    persist int cursor = 0;
                    GUITextEdit(string, STRING_SIZE, &isUsed, &cursor, 0.6);


                GUIBoxBegin("Mouse", V2(0.7, 0));
                    GUIText("Mouse Position");
                    GUIText("     X %f", MousePressedPosition.x);
                    GUIText("     Y %f", MousePressedPosition.y);

                GUIBoxBegin("Debug Test", V2(0, 0.5));
                
                    if(GUIButton("Press me"))
                        showMessage = !showMessage;
                    
                    if(showMessage)
                        GUIText("Secret Message");

                    persist v3 Color = { 0 };
                    GUIColorEdit3f(&Color, 0.3);

                Renderer2DBeginScene(camera->camera, NULL, NULL);

                Renderer2DDrawQuad(
                    &(Quad2D){
                        .color = V4(Color.x, Color.y, Color.z, 1.0f),
                        .height = 1.0f,
                        .width = 1.0f,
                        .position = V2(0.0f, 0.0f)
                    }
                );

                Renderer2DEndScene();
        }
    }
}


bool NeuralOnEvent(const Event* event)
{
    CameraControllerOnEvent(camera, event);
    
    switch(event->type)
    {
        case(MouseMovedEventType):
            MousePressedPosition = event->PosEvent.pos;
            WindowToGUISpace(&MousePressedPosition);
            break;
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
    ApplicationPushLayer(NewLayer(
        (Layer){
            .Init = NeuralInit, 
            .OnUpdate = NeuralOnUpdate, 
            .OnEvent = NeuralOnEvent, 
            .Delete = NeuralDelete
        })
    );
}