#ifndef __GUICONTROLLER_H_
#define __GUICONTROLLER_H_

#include "GUI.h"
#include "src/utils/types.h"
#include "src/renderer/font/font.h"
#include "src/renderer/camera/OrthographicCameraController.h"
#include "src/renderer/textures/Texture2D.h"

typedef enum STYLE_VARS
{
    FONT_SIZE,
    FONT_PADDING,
    WIDGET_MARGIN,
    WIDGET_PADDING
} STYLE_VARS;

typedef struct GUIAssetManager
{
    Font* Font;
    CameraController* CameraController;
    float widgetPadding, widgetMargin; 
    float fontPadding, fontSize, fontSizeHeading;
    float cursorWidth;
    Texture2D* ColorEdit3fTexture;
} GUIAssetManager;

extern void InitGUI();

extern void DeinitGUI();

extern void GUIBegin(float deltaTime);

extern void GUIEnd();

extern GUIAssetManager* GUIGetAssetManager();

extern GUIController* GUIGetController();

extern GUIBox* GUIGetActiveBox();

extern void GUISetActiveBox(GUIBox *box);

extern bool GUIOnEvent(const Event* event);

extern Event GUIGetEvent(u8 EventType);

extern void GUIGetEventEnd(bool deleteLastEvent);


#endif // __GUICONTROLLER_H_