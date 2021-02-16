#ifndef __EVENT_H_
#define __EVENT_H_

#include "../core/window.h"
#include "../n_types.h"

typedef struct
{
    byte handled;
    pos2D pos;
} n_PosEvent;

typedef struct
{
    byte handled;
    uint width;
    uint height;
} n_WindowResize;

typedef struct
{
    uint keyCode;
    byte handled;
    byte action;
}n_ButtonEvent;


typedef struct 
{

    n_PosEvent windowEvent;
    n_PosEvent mouseMoveEvent;
    n_ButtonEvent keyEvent;
    n_ButtonEvent mouseButtonEvent;
    n_ButtonEvent charEvent;
    n_PosEvent scollEvent;
} n_Event;


void n_createEvent(n_Window* this);


#endif // __EVENT_H_