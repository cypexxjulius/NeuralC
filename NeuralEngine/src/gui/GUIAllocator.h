#ifndef __GUI_ALLOCATOR_H_
#define __GUI_ALLOCATOR_H_

#include "GUI.h"
#include "src/utils/types.h"

extern void* GUIAlloca(u16 size);

extern void GUIAllocatorReset();

#endif // __GUI_ALLOCATOR_H_
