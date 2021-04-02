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
} KeyPressedEvent;

typedef struct Event
{
    byte type;
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
    MouseMovedEventType
};

#define Event( _type, _event) (Event){ .type=_type, _event}

#define KeyPressedEvent( _keycode, _action, _mod) (KeyPressedEvent){ _keycode, _action, _mod}

#define WindowResizeEvent( _width, _height) (WindowResizeEvent) {_width, _height}

#define PosEvent( _pos, _delta) (PosEvent) { _pos, _delta}



#endif // __EVENT_STRUCTS_H_