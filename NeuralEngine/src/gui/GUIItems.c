#include "GUIItems.h"
#include "GUIAllocator.h"
#include "GUIController.h"

#include "src/core/error.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include "src/renderer/font/font.h"
#include "src/core/Application.h"
#include <stdarg.h>

float GUIBoxCalculateHeight(GUIBox* box, Font* font, float widgetMargin)
{   
    if(box->Widgets.used == 0)
        return 0;

    float height = box->NameHeight + widgetMargin;

    GUIWidget* widget = NULL;
    for(u32 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        if(widget->String == NULL || widget->textLength == 0)
            continue;

        height += widget->height + widgetMargin * 2;
    }

    return height;
}

float GUIBoxCalculateWidth(GUIBox* box, Font* font, float widgetMargin)
{
    if(box->Widgets.used == 0)
        return 0;

    float width = box->NameWidth;

    GUIWidget* widget = NULL;
    for(u32 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        if(widget->String == NULL || widget->textLength == 0)
            continue;
        
        width = max(width, widget->width);
    }
    

    return( width + widgetMargin  * 2 );
}

void GUIBoxPrepare(GUIBox* box, Font * font, float widgetPadding, float fontPadding, float fontSize)
{
    
    GUIWidget *widget = NULL;
    for(u16 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        widget->height = font->lineHeight * fontSize + 2 * fontPadding;
        widget->width = 2 * fontPadding;

        for(u16 i = 0; i < widget->textLength; i++)
        {
            float GlyphWidth = font->charData[CharToGlyph(widget->String[i])].width;
            widget->width += GlyphWidth * fontSize + font->letterSpacing;
        }
    }

    box->NameHeight = font->lineHeight * fontSize + 2 * fontPadding;
    box->NameWidth = 2 * fontPadding;

    u16 NameLength = (u16)strlen(box->BoxName);    
    for(u16 i = 0; i < NameLength; i++)
    {
        
        float GlyphWidth = font->charData[CharToGlyph(box->BoxName[i])].width;
        box->NameWidth += GlyphWidth * fontSize + font->letterSpacing; 
    }
}

void GUIBoxBegin(const char *boxName, v2 Position)
{
    GUIBox *box = CreateObject(GUIBox);
    
    u16 boxNameLength = (u16)strlen(boxName);
    Assert(boxNameLength >= 50, "boxName length too long");
    Memory.Copy(box->BoxName, boxName, 50);

    
    v2 WindowSize = GetWindowSize();
    float aspectRatio = WindowSize.width / WindowSize.height;

    Position.x *= aspectRatio;
    Position.y *= -aspectRatio;

    Position.x -= aspectRatio;
    Position.y += 1.0f;


    box->Position = Position;

    NewVector(&box->Widgets, 4, sizeof(GUIWidget *), VECTOR_POINTER | VECTOR_FREE);

    GUISetActiveBox(box);
}

void GUIText(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    GUIBox * box = GUIGetActiveBox();

    GUIWidget* widget = CreateObject(GUIWidget);

    widget->textLength = vsnprintf(NULL, 0, format, args) + 1;
    
    widget->String = GUIAlloca(widget->textLength);

    vsnprintf(widget->String, widget->textLength, format, args);

    VectorAdd(&box->Widgets, widget);
    va_end(args);
}