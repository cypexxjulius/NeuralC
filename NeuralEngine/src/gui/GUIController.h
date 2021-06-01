#ifndef __GUICONTROLLER_H_
#define __GUICONTROLLER_H_

#include "GUI.h"
#include "src/utils/types.h"
#include "src/renderer/font/font.h"

typedef enum STYLE_VARS
{
    FONT_SIZE,
    FONT_PADDING,
    WIDGET_MARGIN,
    WIDGET_PADDING
} STYLE_VARS;

extern void InitGUI();

extern void DeinitGUI();

extern void GUIBegin();

extern void GUIEnd();

extern Font* GUIGetFont();

extern void GUIStyleGet(STYLE_VARS var, void *outvariable);

extern GUIBox* GUIGetActiveBox();

extern void GUISetActiveBox(GUIBox *box);

#endif // __GUICONTROLLER_H_