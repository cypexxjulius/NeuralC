#include "GUIController.h"
#include "src/platform/memory.h"
#include "src/core/error.h"
#include "src/utils/types.h"
#include "GUIAllocator.h"

extern void GUIRenderBox(GUIBox* box);

typedef struct GUIAssetManager
{
    Font* Font;
    float widgetPadding, widgetMargin; 
    float fontPadding;
} GUIAssetManager;

static GUIController Controller;
static GUIAssetManager AssetManager;


void InitGUI()
{
    Controller = (GUIController){ 0 };
    AssetManager = (GUIAssetManager) { 0 };

    NewVector(&Controller.boxes, 2, sizeof(GUIBox * ), VECTOR_POINTER | VECTOR_FREE);

    AssetManager.Font = NewFontTexture("res/fonts/Roboto-Black.ttf");

    AssetManager.widgetPadding = 1.0f;
    AssetManager.fontPadding = 0.25f;
    AssetManager.widgetMargin = 2.0f;
}

void DeinitGUI()
{
    DeleteFont(AssetManager.Font);
}

void GUIBegin()
{
    GUIAllocatorReset();
}

void GUIEnd()
{    
    GUIBox* box;
    for(u16 i = 0; i < Controller.boxes.used; i++)
    {
        box = VectorGet(&Controller.boxes, i);

        if(box != NULL && box->Widgets.used != 0)
            GUIRenderBox(box);

        VectorClear(&box->Widgets);
    }

    VectorClear(&Controller.boxes);
}

Font* GUIGetFont()
{
    return AssetManager.Font;
}

void GUIStyleGet(float *outWidgetPadding, float *outFontPadding, float *widgetMargin)
{
    if(outFontPadding != NULL)
        *outFontPadding = AssetManager.fontPadding;

    if(outWidgetPadding != NULL)
        *outWidgetPadding = AssetManager.widgetPadding;

    if(widgetMargin != NULL)
        *widgetMargin = AssetManager.widgetMargin;
}

GUIBox* GUIGetActiveBox()
{
    return VectorGet(&Controller.boxes, Controller.boxes.used - 1);
}

void GUISetActiveBox(GUIBox *box)
{
    VectorAdd(&Controller.boxes, box);
}