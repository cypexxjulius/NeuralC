#ifndef __EVENT_H_
#define __EVENT_H_

struct n_Window;

enum n_EVENT_TYP
{
    NO_EVENT,
    SCROLL_EVENT,
    WINDOW_RESIZE_EVENT,
    WINDOW_CLOSE_EVENT,
    KEY_CALLBACK_EVENT,
    CHAR_CALLBACK_EVENT,
    MOUSE_BUTTON_EVENT,
    MOUSE_MOVED_EVENT,
    MAX_EVENTS
};


void n_initEvent(struct n_Window* this);


#endif // __EVENT_H_