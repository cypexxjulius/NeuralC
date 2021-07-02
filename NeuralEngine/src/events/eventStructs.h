#ifndef __EVENT_STRUCTS_H_
#define __EVENT_STRUCTS_H_


#include "src/utils/types.h"

typedef struct PosEvent
{
    v2 pos;
    v2 delta;
} PosEvent;

typedef struct WindowResizeEvent
{
    i16 width;
    i16 height;
} WindowResizeEvent;

typedef struct KeyPressedEvent
{
    u32 keyCode;
    u8 action;
    u8 mod;
    v2 position;
} KeyPressedEvent;


typedef struct Event
{
    byte type;
    byte Cancable;
    union 
    {
        u32 id;
        u32 valid;
    };
    
    union
    { 
        KeyPressedEvent KeyPressedEvent;
        WindowResizeEvent WindowResizeEvent;
        PosEvent PosEvent;
        u8 close;
    };

} Event;

enum EventTypes
{
    WindowResizeEventType = 1,
    WindowCloseEventType,
    KeyPressedEventType,
    MouseButtonPressedEventType,
    ScrolledEventType,
    MouseMovedEventType,
    CharEventType,
};


enum KeyMods
{
    NL_SHIFT_MOD = 2
};

#endif // __EVENT_STRUCTS_H_