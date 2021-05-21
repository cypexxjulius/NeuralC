#ifndef __GUIITEMS_H_
#define __GUIITEMS_H_

#include "GUI.h"

extern void GUIBoxAddWidget(GUIBox* box, WidgetType type, void* data);

extern float GUIBoxCalculateHeight(GUIBox* box, Font* font, float widgetMargin);

extern float GUIBoxCalculateWidth(GUIBox* box, Font* font, float widgetMargin);

extern void GUIBoxPrepare(GUIBox* box, Font * font, float widgetPadding, float fontPadding);

#endif // __GUIITEMS_H_