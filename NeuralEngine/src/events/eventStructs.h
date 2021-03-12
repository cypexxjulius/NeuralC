#ifndef __EVENTSTRUCTS_H_
#define __EVENTSTRUCTS_H_


#include "src/utils/types.h"

typedef struct
{
    v2 pos;
} n_PosEvent;

typedef struct
{
    u32 width;
    u32 height;
} n_WindowResizeEvent;

typedef struct
{
    u32 keyCode;
    byte action;
}n_ButtonEvent;

typedef struct Event_Dispatcher
{
    byte type;
    n_ButtonEvent keyEvent;
    n_WindowResizeEvent windowResizeEvent;
    n_PosEvent posEvent;
}n_EventDispatcher;

#endif // !__EVENTSTRUCTS_H_



