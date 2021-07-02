#include "src/renderer/Renderer2D.h"
#include "GUIController.h"
#include "src/platform/memory.h"
#include "src/core/error.h"
#include "src/utils/types.h"
#include "GUIAllocator.h"
#include "src/core/Input.h"
#include "src/utils/Logger.h"
#include "src/core/Application.h"

extern void GUIRenderBox(GUIBox* box, GUIAssetManager* AssetManager, GUIController* Controller);

static GUIController Controller;
static GUIAssetManager AssetManager;

#define EVENT_MAX_BUFFER 32
static u8 EventBufferIndex = 0;
static u8 EventBufferPointerTemp = 0; 
static Event EventBuffer[EVENT_MAX_BUFFER + 1] = { 0 };


void InitGUI()
{
    Controller = (GUIController){ 0 };
    AssetManager = (GUIAssetManager) { 0 };

    NewVector(&Controller.boxes, 2, sizeof(GUIBox * ), VECTOR_POINTER | VECTOR_FREE);
    Controller.startTime = 0.0f;

    AssetManager.CameraController = NewOrthographicCameraController(CameraNoControls);
    
    AssetManager.Font = NewFontTexture("res/fonts/Roboto-Black.ttf");

    AssetManager.widgetPadding = 0.01f;
    AssetManager.fontPadding = 0.01f;
    AssetManager.widgetMargin = 0.02f;
    AssetManager.fontSize = 0.03f;
    AssetManager.fontSizeHeading = AssetManager.fontSize * 1.1f;
    AssetManager.cursorWidth = AssetManager.fontSize * 0.2f;

    AssetManager.ColorEdit3fTexture = NewTexture2DEmpty(255, 255, Image_TypeRGBA);

    u8 ImageBuffer[255][255][4];

    for(unsigned int i = 0; i < 255; i++)
    {
        for(u16 k = 0; k < 255; k++)
        {
            ImageBuffer[i][k][0] = (u8)i;
            ImageBuffer[i][k][1] = (u8)k; 
            ImageBuffer[i][k][2] = (u8)255 - k; 
            ImageBuffer[i][k][3] = (u8)255; 
            
        }
    }

    Texture2DSetData(AssetManager.ColorEdit3fTexture, ImageBuffer, 255 * 255 * 4);


}

void DeinitGUI()
{
    DeleteOrthographicCameraController(AssetManager.CameraController);
    DeleteFont(AssetManager.Font);
}

void GUIBegin(float deltaTime)
{
    Controller.startTime += deltaTime;
    GUIAllocatorReset();
}

void GUIEnd()
{    
    Renderer2DBeginScene(AssetManager.CameraController->camera, NULL, AssetManager.Font);

    for(u16 i = 0; i < Controller.boxes.used; i++)
    {
        GUIBox* box = VectorGet(&Controller.boxes, i);

        if(box != NULL)
            GUIRenderBox(box, &AssetManager, &Controller);


        VectorClear(&box->Widgets);
    }
    
    Renderer2DEndScene();
    
    /*
    Passing Events down the event chain
    */

    for(u8 i = 0; i < EventBufferIndex; i++)
    {
        if(EventBuffer[EventBufferIndex].valid == false)
            continue;

        bool shouldCancel = ApplicationEventAfterGUI(&EventBuffer[EventBufferIndex]);


        if( shouldCancel && EventBuffer[EventBufferIndex].Cancable)
            EventBuffer[EventBufferIndex].valid = false;
    }
    EventBufferIndex = 0;
        
    
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


GUIBox* GUIGetActiveBox()
{
    return VectorGet(&Controller.boxes, Controller.boxes.used - 1);
}

/*

Event processing and handling

*/


bool GUIOnEvent(const Event* event)
{
    CameraControllerOnEvent(AssetManager.CameraController, event);


    if(EventBufferIndex <= EVENT_MAX_BUFFER)
        EventBuffer[EventBufferIndex++] = *event;

    return false;
}

Event GUIGetEvent(u8 EventType)
{
    if(EventBufferIndex == 0)
        return (Event) { 0 };

    for(u8 i = EventBufferPointerTemp; i < EventBufferIndex; i++)
    {

        if(EventType == EventBuffer[i].type && EventBuffer[i].valid)
        {
            EventBufferPointerTemp = i + 1;
            return EventBuffer[i];
        }
    }

    EventBufferPointerTemp = 0;
    return (Event){ 0 };
}

void GUIGetEventEnd(bool deleteLastEvent)
{
    if(EventBufferPointerTemp == 0)
        return;

    if(deleteLastEvent)
    {
        LOG("Deleted Event %u", EventBuffer[EventBufferPointerTemp - 1].type);
        EventBuffer[EventBufferPointerTemp - 1].valid = false;
    }
    EventBufferPointerTemp = 0; 
}



