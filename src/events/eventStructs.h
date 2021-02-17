#ifndef __EVENTSTRUCTS_H_
#define __EVENTSTRUCTS_H_


#include "../n_types.h"

typedef struct
{
    float2 pos;
} n_PosEvent;

typedef struct
{
    uint width;
    uint height;
} n_WindowResizeEvent;

typedef struct
{
    uint keyCode;
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



