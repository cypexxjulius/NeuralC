#include "src/renderer/Renderer2D.h"
#include "GUIController.h"
#include "src/platform/memory.h"
#include "src/core/error.h"
#include "src/utils/types.h"
#include "GUIAllocator.h"
#include "src/core/Input.h"
#include "src/utils/Logger.h"

extern void GUIRenderBox(GUIBox* box);

static GUIController Controller;
static GUIAssetManager AssetManager;


void InitGUI()
{
    Controller = (GUIController){ 0 };
    AssetManager = (GUIAssetManager) { 0 };

    NewVector(&Controller.boxes, 2, sizeof(GUIBox * ), VECTOR_POINTER | VECTOR_FREE);
    AssetManager.CameraController = NewOrthographicCameraController(CameraNoControls);
    
    AssetManager.Font = NewFontTexture("res/fonts/Roboto-Black.ttf");

    AssetManager.widgetPadding = 0.01f;
    AssetManager.fontPadding = 0.01f;
    AssetManager.widgetMargin = 0.01f;
    AssetManager.fontSize = 0.03f;
}

void DeinitGUI()
{
    DeleteOrthographicCameraController(AssetManager.CameraController);
    DeleteFont(AssetManager.Font);
}

void GUIBegin()
{
    GUIAllocatorReset();
}

void GUIEnd()
{    
    Renderer2DBeginScene(AssetManager.CameraController->camera, NULL, AssetManager.Font);

    for(u16 i = 0; i < Controller.boxes.used; i++)
    {
        GUIBox* box = VectorGet(&Controller.boxes, i);

        if(box != NULL && box->Widgets.used != 0)
            GUIRenderBox(box);


        VectorClear(&box->Widgets);
    }
    
    Renderer2DEndScene();

    Controller.MousePressedEvent.Position = V2(0, 0);
    
    for(u32 i = 0; i < Controller.boxes.used; i++)
    {
        GUIBox* box = VectorGet(&Controller.boxes, i);
        DeleteVector(&box->Widgets);
    }

    VectorClear(&Controller.boxes);
}

GUIAssetManager* GUIGetAssetManager()
{
    return &AssetManager;
}

GUIController* GUIGetController()
{
    return &Controller;
}

bool GUIOnEvent(const Event* event)
{
    CameraControllerOnEvent(AssetManager.CameraController, event);

    switch (event->type)
    {
    case MouseButtonPressedEventType:
        
        if(event->KeyPressedEvent.keyCode == NL_MOUSE_BUTTON_LEFT)
        {

            v2 MousePosition = InputGetMousePosition();
            u8 isPressed = event->KeyPressedEvent.action == KeyState_Pressed;
            MousePosition.x *= isPressed;
            MousePosition.y *= isPressed;

            LOG("GUIMouse %u, { %f %f}", event->KeyPressedEvent.action, MousePosition.x, MousePosition.y);

            Controller.MousePressedEvent.Position = MousePosition;
        }
        break;
    }

    return false;
}


GUIBox* GUIGetActiveBox()
{
    return VectorGet(&Controller.boxes, Controller.boxes.used - 1);
}

void GUISetActiveBox(GUIBox *box)
{
    VectorAdd(&Controller.boxes, box);
}

