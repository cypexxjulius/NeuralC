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
    GUIWidgetTypePlain,
    GUIWidgetTypeText,
    GUIWidgetTypeTextEdit,
    GUIWidgetTypeButton,
    GUIWidgetTypeColorEdit3f
} WidgetType;

typedef struct GUIWidget
{
    float width, height;
    WidgetType type;

    u16 textLength;
    void *Data;
    u16 cursorPos;

    u8 isPressed;
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
    float startTime;
} GUIController;

extern void WindowToGUISpace(v2 *Position);

extern void GUItoRenderSpace(v2 *Position);

#include "GUIController.h"
#include "GUIItems.h"

#endif // __GUIMODULES_H_