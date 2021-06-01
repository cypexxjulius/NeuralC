#ifndef __GUIMODULES_H_
#define __GUIMODULES_H_

#include "src/utils/types.h"
#include "src/renderer/textures/Texture2D.h"
#include "src/events/eventStructs.h"

// Upcoming GUI Widgets
// Button
// Checkbox
// Textfield
// Change vec4, vec3, vec2

#define PAGE_NAME_LENGTH 50
#define BOX_NAME_LENGTH 50

typedef enum WidgetType
{
    WidgetTypePlain,
    WidgetTypeText,
} WidgetType;

typedef struct GUIWidget
{
    WidgetType type;
    u16 textLength;
    char *String;
    float width, height;
} GUIWidget;

typedef struct GUIBox
{

    v2 Position;
    Vector Widgets;
    float height, width;
    float NameWidth, NameHeight;
    char BoxName[BOX_NAME_LENGTH];

} GUIBox;


typedef struct GUIController {
    Vector boxes;
} GUIController;

void GUIBoxBegin(const char *boxName, v2 Position);

void GUIText(const char *format, ...);

#include "GUIController.h"

#endif // __GUIMODULES_H_