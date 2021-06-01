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
    float fontPadding, fontSize;
} GUIAssetManager;

static GUIController Controller;
static GUIAssetManager AssetManager;


void InitGUI()
{
    Controller = (GUIController){ 0 };
    AssetManager = (GUIAssetManager) { 0 };

    NewVector(&Controller.boxes, 2, sizeof(GUIBox * ), VECTOR_POINTER | VECTOR_FREE);

    AssetManager.Font = NewFontTexture("res/fonts/Roboto-Black.ttf");

    AssetManager.widgetPadding = 0.01f;
    AssetManager.fontPadding = 0.01f;
    AssetManager.widgetMargin = 0.01f;
    AssetManager.fontSize = 0.3f;
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

void GUIStyleGet(STYLE_VARS var, void *outvariable)
{
    switch(var)
    {
        case FONT_SIZE:
            *(float *)outvariable = AssetManager.fontSize;
            return;
        
        case FONT_PADDING:
            *(float *)outvariable = AssetManager.fontPadding;
            return;
        
        case WIDGET_MARGIN:
            *(float *)outvariable = AssetManager.widgetMargin;
            return;
        
        case WIDGET_PADDING:
            *(float *)outvariable = AssetManager.widgetPadding;
            return;
    }
}

GUIBox* GUIGetActiveBox()
{
    return VectorGet(&Controller.boxes, Controller.boxes.used - 1);
}

void GUISetActiveBox(GUIBox *box)
{
    VectorAdd(&Controller.boxes, box);
}