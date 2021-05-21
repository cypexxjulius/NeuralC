#ifndef __GUICONTROLLER_H_
#define __GUICONTROLLER_H_

#include "GUI.h"
#include "src/utils/types.h"
#include "src/renderer/font/font.h"

extern void InitGUI();

extern void DeinitGUI();

extern void GUIBegin();

extern void GUIEnd();

extern Font* GUIGetFont();

extern void GUIStyleGet(float *outWidgetPadding, float *outFontPadding, float *widgetMargin);

extern GUIBox* GUIGetActiveBox();

extern void GUISetActiveBox(GUIBox *box);

#endif // __GUICONTROLLER_H_