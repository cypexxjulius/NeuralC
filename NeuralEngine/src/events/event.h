#ifndef __EVENT_H_
#define __EVENT_H_

#include "eventStructs.h"

struct Window;

typedef enum KeyState
{
    KeyState_Released,
    KeyState_Pressed,
    KeyState_Repeated
} KeyState;


void InitEventSystem(struct Window* this);

#endif // __EVENT_H_