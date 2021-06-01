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

static char string[4096] = { 0 };
static unsigned int stringCursor = 0;
static v2 MousePressedPosition = { 0 };


void NeuralInit()
{   
    ApplicationCreateWindow(1280, 720, "Test Window"); 

    camera = NewOrthographicCameraController(CameraMouseDragController | CameraMouseScrollSensitive);

    texture = NewTexture2D("res/textures/Checkerboard.png");
    texture2 = NewTexture2D("res/textures/firstImage.jpg");
}

void NeuralGUIUpdate(float deltaTime)
{
    GUIBoxBegin("Test", V2(0, 0));

        GUIText("FrameTime %.2fms", 1000.0f * deltaTime);
        GUIText("FPS %.2f", 1.0f / deltaTime);



    GUIBoxBegin("Mouse", V2(1.0, 0.9));

        GUIText("Mouse Position");
        GUIText("     X %f", MousePressedPosition.x);
        GUIText("     Y %f", MousePressedPosition.y);
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
            Renderer2DBeginScene(camera->camera);
                        
            
            Renderer2DDrawQuad(&(Quad2D) {
                .position = V2(0.0f, 0.0f),
                .width = 0.1f,
                .height = 0.1f,
                .color = V4(1.0, 0.3, 0.1, 1.0),
            });


            static float lineThickness = 0.001;

            
            Renderer2DDrawQuad(&(Quad2D) {
                .position = V2(0, 0),
                .color = V4(1.0f, 1.0f, 1.0f, 1.0f),
                .width = lineThickness * 500,
                .height = lineThickness,
            });

            Renderer2DDrawQuad(&(Quad2D) {
                .position = V2(0, 0),
                .color = V4(1.0f, 1.0f, 1.0f, 1.0f),
                .width = lineThickness,
                .height = lineThickness * 500,
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
                    if(event->KeyPressedEvent.action == 0)
                        string[stringCursor++] = '\n';   
                    
                    break;
            }
            break;

        case(CharEventType) :
            char keycode = (char)event->KeyPressedEvent.keyCode;

            string[stringCursor++] = keycode;
            string[stringCursor] = 0;
            break;
        
        case (MouseMovedEventType):
            MousePressedPosition = event->PosEvent.pos;
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
            .Delete = NeuralDelete,
            .GUIUpdate = NeuralGUIUpdate
        })
    );
}