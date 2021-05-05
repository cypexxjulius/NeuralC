#include <stdio.h>
#include <string.h>

#include <Neural.h>

#include "src/utils/Timer.h"

static CameraController* camera = NULL;

static Texture2D* texture = NULL;
static Texture2D* texture2 = NULL;
static Texture2D* CharSet = NULL;

static float Rotation = 0.0f;

#define maxSize 100

static char string[4096] = { 0 };
static unsigned int stringCursor = 0;

void NeuralInit()
{
    unsigned int width = 1280, height = 720;
    ApplicationCreateWindow(width, height, "Test Window"); 

    camera = NewOrthographicCameraController(CameraAllControls);

    texture = NewTexture2D("res/textures/Checkerboard.png");
    texture2 = NewTexture2D("res/textures/firstImage.jpg");
    CharSet = NewTexture2D("res/textures/CharSet2.png");
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
                .position = v2(0.0f, 0.0f), 
                .scale = v2(20.0f, 20.0f), 
                .texture = texture,
                .tiling = 10.0f,
                // .rotation = Rotation,
            });

            Rotation += deltaTime;

            
            Renderer2DEndScene();

            Renderer2DBeginScene(NULL);
            {

                Renderer2DDrawQuad((Quad2D) {
                    .position = v2(0.0, 0.0),
                    .scale = v2(0.1, 0.1),
                    .texture = texture
                });

                Renderer2DDrawQuad((Quad2D) {
                    .position = v2(-1.0f, -0.5f),
                    .color = v4(0.0f, 1.0f, 1.0f, 1.0f),
                    .text = (TextElement []) {
                        (TextElement) {
                            .string = string,
                            .color = v3(1.0, 0.3, 0.2),
                            .fontSize = 0.05f
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

            if(keycode >= 'a' && keycode <= 'z')
                keycode -= 'a' - 'A';
            
            if(keycode >= ' ' && keycode <= 'Q')
            {
                string[stringCursor++] = keycode;
                string[stringCursor] = 0;
            }
            puts(string);
        }
    }
    return 0;
}


void NeuralDelete()
{
    DeleteTexture2D(texture);
    DeleteTexture2D(CharSet);
    DeleteTexture2D(texture2);
    DeleteOrthographicCameraController(camera);
}

void CreateApplication()
{
    NewApplication("Test Game");
    ApplicationPushLayer(NewLayer(NeuralInit, NeuralOnUpdate, NeuralOnEvent, NeuralDelete));
}