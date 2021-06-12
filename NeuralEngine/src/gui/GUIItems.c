#include "GUIItems.h"
#include "GUIAllocator.h"
#include "GUIController.h"


#include "src/core/error.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include "src/renderer/font/font.h"
#include "src/core/Input.h"
#include "src/renderer/Renderer2D.h"
#include "src/utils/Logger.h"

#include <stdarg.h>

void GUIBoxBegin(const char *boxName, v2 Position)
{
    GUIController *controller = GUIGetController(); 
    GUIBox *box = CreateObject(GUIBox);
    
    u16 boxNameLength = (u16)strlen(boxName);

    Assert(boxNameLength >= 50, "boxName length too long");
    Memory.Copy(box->BoxName, boxName, 50);

    
    GUItoRenderSpace(&Position);


    box->Position = Position;

    GUIAssetManager* AssetManager = GUIGetAssetManager();

    box->NameHeight = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    box->NameWidth += 2 * AssetManager->fontPadding * Renderer2DTextLength(AssetManager->Font, box->BoxName, AssetManager->fontSize); 

    box->width = box->NameWidth;
    box->height = box->NameHeight + AssetManager->widgetMargin;


    NewVector(&box->Widgets, 4, sizeof(GUIWidget *), VECTOR_POINTER | VECTOR_FREE);

    VectorAdd(&controller->boxes, box);
}

void GUIBoxEnd() {}

void GUIText(const char *format, ...)
{
    if(format == NULL)
        return;


    va_list args;
    va_start(args, format);

    GUIBox * box = GUIGetActiveBox();
    GUIAssetManager* AssetManager = GUIGetAssetManager();

    GUIWidget* widget = CreateObject(GUIWidget);
    widget->type = GUIWidgetTypeText;

    u16 textLength = vsnprintf(NULL, 0, format, args) + 1;

    widget->String = GUIAlloca(textLength);

    vsnprintf(widget->String, textLength, format, args);
    va_end(args);


    widget->height = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    widget->width = 2 * AssetManager->fontPadding + Renderer2DTextLength(AssetManager->Font, widget->String, AssetManager->fontSize);


    box->height += widget->height + AssetManager->widgetMargin * 2;
    box->width = max(box->width, widget->width + 2 * AssetManager->widgetMargin);


    VectorAdd(&box->Widgets, widget);
}

bool GUIButton(const char *name)
{
    GUIBox * box = GUIGetActiveBox();
    GUIAssetManager* AssetManager = GUIGetAssetManager();

    GUIWidget* widget = CreateObject(GUIWidget);
    widget->type = GUIWidgetTypeButton;
    
    // Copying the name to the widget struct
    u16 textLength = (u16)strlen(name) + 1;
    widget->String = GUIAlloca(textLength);
    Memory.Copy(widget->String, name, textLength);

    widget->height = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    widget->width = 2 * AssetManager->fontPadding + Renderer2DTextLength(AssetManager->Font, widget->String, AssetManager->fontSize);

    GUIController* Controller = GUIGetController();
    v2 MousePosition = Controller->MousePressedEvent.Position;
    if(MousePosition.x && MousePosition.y)
    {       
        WindowToGUISpace(&MousePosition);


        v2 WidgetPosition = V2(
            box->Position.x + AssetManager->widgetMargin,  
            box->Position.y - (box->height - box->NameHeight)
        );


        widget->isPressed = (MousePosition.x >= WidgetPosition.x) 
                            && (MousePosition.x <= (WidgetPosition.x + widget->width))
                            && (MousePosition.y <= WidgetPosition.y + widget->height)
                            && (MousePosition.y >= WidgetPosition.y);
    }


    box->height += widget->height + AssetManager->widgetMargin * 2;
    box->width = max(box->width, widget->width + 2 * AssetManager->widgetMargin);

    VectorAdd(&box->Widgets, widget);

    return widget->isPressed;
}