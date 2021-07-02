#ifndef __GUIITEMS_H_
#define __GUIITEMS_H_

#include "GUI.h"


extern void GUIBoxBegin(const char *boxName, v2 Position);

extern bool GUIButton(const char *name);

extern void GUIText(const char *format, ...);

extern bool GUITextEdit(char *stringBuffer, u32 maxLength, bool *isUsed, int *cursor, float width);

extern bool GUIColorEdit3f(v3* color, float Size);

#endif // __GUIITEMS_H_