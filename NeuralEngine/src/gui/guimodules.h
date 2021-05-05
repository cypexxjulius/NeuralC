#ifndef __GUIMODULES_H_
#define __GUIMODULES_H_

#include "src/utils/types.h"

// Upcoming GUI Widgets
// Button
// Checkbox
// Textfield
// Change vec4, vec3, vec2

typedef struct BoxWidget
{
    v4 color;
    v2 position;
    v2 scale;
    float zIndex;

} BoxWidget;

typedef struct TextBoxWidget
{
    char **buffer;
    u32 options;
    v4 color;
    v2 position;

} TextBoxWidget;

typedef enum WigetType
{
    NotAssignedType,
    BoxWidgetType,
    TextBoxWidgetType,
} WigetType;

typedef struct Widget
{
    WigetType type;
    u32 id;
    union
    {
        BoxWidget BoxWidget;
        TextBoxWidget TextBoxWidget;
    };
} Widget;


#endif // __GUIMODULES_H_