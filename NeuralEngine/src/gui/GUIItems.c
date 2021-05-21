#include "GUIItems.h"
#include "GUIAllocator.h"
#include "GUIController.h"

#include "src/core/error.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"
#include "src/renderer/font/font.h"
#include <stdarg.h>

float GUIBoxCalculateHeight(GUIBox* box, Font* font, float widgetMargin)
{   
    if(box->Widgets.used == 0)
        return 0;

    float height = 2 * widgetMargin;

    GUIWidget* widget = NULL;
    for(u32 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        if(widget->String == NULL || widget->textLength == 0)
            continue;

        height += widget->height + widgetMargin;
    }

    return height;
}

float GUIBoxCalculateWidth(GUIBox* box, Font* font, float widgetMargin)
{
    if(box->Widgets.used == 0)
        return 0;

    float width = widgetPadding  * 2;

    GUIWidget* widget = NULL;
    for(u32 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        if(widget->String == NULL || widget->textLength == 0)
            continue;

        float tempWidth = widget->width;

        width = max(width, tempWidth);
    }
    

    return width;
}

void GUIBoxPrepare(GUIBox* box, Font * font, float widgetPadding, float fontPadding)
{
    
    GUIWidget *widget = NULL;
    for(u16 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        widget->height = font->lineHeight + 2 * fontPadding;
        widget->width = 2 * fontPadding;

        for(u16 i = 0; i < widget->textLength; i++)
        {
            v2 Scale;
            FontGetCharInfo(font, widget->String[i], NULL, &Scale, NULL);
            widget->width += Scale.width + font->letterSpacing;
        }
    }
}

void GUIBoxBegin(const char *boxName, v2 Position)
{
    GUIBox *box = CreateObject(GUIBox);
    
    u16 boxNameLength = strlen(boxName);
    Assert(boxNameLength >= 50, "boxName length too long");
    Memory.Copy(box->BoxName, boxName, 50);

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

    widget->textLength = vsnprintf(NULL, 0, format, args);
    
    widget->String = GUIAlloca(widget->textLength);

    vsnprintf(widget->String, widget->textLength, format, args);

    VectorAdd(&box->Widgets, widget);
    va_end(args);
}