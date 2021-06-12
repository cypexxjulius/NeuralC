#ifndef __GUIITEMS_H_
#define __GUIITEMS_H_

#include "GUI.h"

extern void GUIText(const char *format, ...);

extern void GUIBoxBegin(const char *boxName, v2 Position);

extern bool GUIButton(const char *name);

#endif // __GUIITEMS_H_