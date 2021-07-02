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

    box->NameHeight     = AssetManager->Font->lineHeight * AssetManager->fontSizeHeading + 2 * AssetManager->fontPadding;
    box->NameWidth      = 2 * AssetManager->fontPadding + Renderer2DTextLength(AssetManager->Font, box->BoxName, AssetManager->fontSizeHeading, 0); 

    box->width          = box->NameWidth;
    box->height         = box->NameHeight + AssetManager->widgetMargin;


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

    widget->Data = GUIAlloca(textLength);

    vsnprintf(widget->Data, textLength, format, args);
    va_end(args);


    widget->height  = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    widget->width   = 2 * AssetManager->fontPadding + Renderer2DTextLength(AssetManager->Font, widget->Data, AssetManager->fontSize, 0);


    box->height     += widget->height + AssetManager->widgetMargin * 2;
    box->width      = max(box->width, widget->width + 2 * AssetManager->widgetMargin);


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
    widget->Data = GUIAlloca(textLength);
    Memory.Copy(widget->Data, name, textLength);

    widget->height = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    widget->width = 2 * AssetManager->fontPadding + Renderer2DTextLength(AssetManager->Font, widget->Data, AssetManager->fontSize, 0);

    Event MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    
    while(MousePressedEvent.valid != 0 && MousePressedEvent.KeyPressedEvent.action == KeyState_Pressed)
    {
        v2 MousePosition = MousePressedEvent.KeyPressedEvent.position; 
        WindowToGUISpace(&MousePosition);



        v2 WidgetPosition = V2(
            box->Position.x + AssetManager->widgetMargin,  
            box->Position.y  - (box->height + AssetManager->widgetMargin) 
        );

        widget->isPressed = (MousePosition.x >= WidgetPosition.x) 
                                * (MousePosition.x <= (WidgetPosition.x + widget->width))
                                * (MousePosition.y <= WidgetPosition.y)
                                * (MousePosition.y >= WidgetPosition.y  - widget->height);

        if(widget->isPressed == true)
            break;
        else 
            MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    }

    GUIGetEventEnd(widget->isPressed);


    box->height += widget->height + AssetManager->widgetMargin * 2;
    box->width = max(box->width, widget->width + 2 * AssetManager->widgetMargin);

    VectorAdd(&box->Widgets, widget);

    return widget->isPressed;
}


bool GUITextEdit(char *stringBuffer, u32 maxLength, bool *isUsed, int *cursor, float width)
{

    Assert((*cursor) >= (int)maxLength, "Cursor overflowed buffer");
    
    GUIAssetManager* AssetManager = GUIGetAssetManager();
    GUIWidget* widget = CreateObject(GUIWidget);
    GUIBox * box = GUIGetActiveBox();

    widget->type = GUIWidgetTypeTextEdit;
    
    widget->height = AssetManager->Font->lineHeight * AssetManager->fontSize + 2 * AssetManager->fontPadding;
    widget->width = 2 * AssetManager->fontPadding + width;

    widget->isPressed = *isUsed;
    
    Event MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    
    while(MousePressedEvent.valid != 0 && MousePressedEvent.KeyPressedEvent.action == KeyState_Pressed)
    {
        v2 MousePosition = MousePressedEvent.KeyPressedEvent.position; 
        WindowToGUISpace(&MousePosition);



        v2 WidgetPosition = V2(
            box->Position.x + AssetManager->widgetMargin,  
            box->Position.y  - (box->height + AssetManager->widgetMargin) 
        );

        widget->isPressed =     (MousePosition.x >= WidgetPosition.x) 
                                && (MousePosition.x <= (WidgetPosition.x + widget->width))
                                && (MousePosition.y <= WidgetPosition.y)
                                && (MousePosition.y >= WidgetPosition.y  - widget->height);

        *isUsed = (bool)widget->isPressed;

        MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    }

    GUIGetEventEnd(widget->isPressed);

    if(widget->isPressed == true)
    {
        Event TextEvent = GUIGetEvent(CharEventType);

        while(TextEvent.valid)
        {
            
            if((*cursor) >= (int)maxLength - 1)
                break;

            if(Renderer2DTextLength(AssetManager->Font, stringBuffer, AssetManager->fontSize, 0) >= width - AssetManager->cursorWidth)
                break;

            stringBuffer[*cursor] = TextEvent.KeyPressedEvent.keyCode;
            stringBuffer[(*cursor) + 1] = '\0';
            (*cursor)++;


            GUIGetEventEnd(true);
            TextEvent = GUIGetEvent(CharEventType);
        }

        TextEvent = GUIGetEvent(KeyPressedEventType);

        while(TextEvent.valid)
        {
            switch (TextEvent.KeyPressedEvent.keyCode)
            {

                case NL_KEY_BACKSPACE:

                    if(*cursor == 0 || TextEvent.KeyPressedEvent.action == 0)
                        goto endInput;

                    if(TextEvent.KeyPressedEvent.mod == NL_SHIFT_MOD)
                    {
                        stringBuffer[0] = '\0';
                        *cursor = 0;
                        break;
                    }

                    stringBuffer[--(*cursor)] = 0;
                    break;    

                case NL_KEY_ENTER:

                    *isUsed = widget->isPressed = false;
                    break;
            }

            GUIGetEventEnd(true);
            TextEvent = GUIGetEvent(CharEventType);
        }

    }
    
    endInput:;

    // Copying the name to the widget struct
    u16 textLength = (u16)strlen(stringBuffer) + 1;
    widget->Data = GUIAlloca(textLength);
    Memory.Copy(widget->Data, stringBuffer, textLength);

    widget->cursorPos = *cursor;


    box->height += widget->height + AssetManager->widgetMargin * 2;
    box->width = max(box->width, widget->width + 2 * AssetManager->widgetMargin);

    VectorAdd(&box->Widgets, widget);

    return widget->isPressed;
}

bool GUIColorEdit3f(v3* color, float Size)
{
    GUIAssetManager* AssetManager = GUIGetAssetManager();
    GUIWidget* widget = CreateObject(GUIWidget);
    GUIBox * box = GUIGetActiveBox();

    widget->type = GUIWidgetTypeColorEdit3f;
    widget->Data = color;

    widget->height = widget->width = 2 * AssetManager->widgetPadding + Size;

    widget->width += AssetManager->widgetPadding * 2 + Size / 5;

    Event MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    
    while(MousePressedEvent.valid != 0 && MousePressedEvent.KeyPressedEvent.action == KeyState_Pressed)
    {
        v2 MousePosition = MousePressedEvent.KeyPressedEvent.position; 
        WindowToGUISpace(&MousePosition);



        v2 WidgetPosition = V2(
            box->Position.x + AssetManager->widgetMargin,  
            box->Position.y  - (box->height + AssetManager->widgetMargin) 
        );

        widget->isPressed =     (MousePosition.x >= WidgetPosition.x) 
                                && (MousePosition.x <= (WidgetPosition.x + widget->width))
                                && (MousePosition.y <= WidgetPosition.y)
                                && (MousePosition.y >= WidgetPosition.y  - widget->height);

        float xpos = (MousePosition.x   - WidgetPosition.x) / widget->width;
        float ypos = (WidgetPosition.y  - MousePosition.y) / widget->height;  

        if(widget->isPressed) *color = (v3){
            ypos,
            xpos,
            1 - xpos     
        }; 

        MousePressedEvent = GUIGetEvent(MouseButtonPressedEventType);
    }

    GUIGetEventEnd(widget->isPressed);


    box->height += widget->height + AssetManager->widgetMargin * 2;
    box->width = max(box->width, widget->width + 2 * AssetManager->widgetMargin);

    VectorAdd(&box->Widgets, widget);

    return widget->isPressed;
 
}