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

static const float widgetPadding = 0.75;

static const float fontHeight = 1;
static const float fontPadding = 0.5;

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
    u8 height, width;
    char BoxName[BOX_NAME_LENGTH];

} GUIBox;


typedef struct GUIController {
    Vector boxes;
} GUIController;

void GUIBoxBegin(const char *boxName, v2 Position);

void GUIText(const char *format, ...);

#include "GUIController.h"

#endif // __GUIMODULES_H_