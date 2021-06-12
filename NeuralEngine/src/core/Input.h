#ifndef __INPUT_H_
#define __INPUT_H_

#include "src/utils/types.h"
#include "src/events/keycode.h"
#include "src/events/event.h"

extern v2 InputGetMousePosition();
extern int InputIsButtonPressed(int key);
extern int InputIsMouseButtonPressed(int key);
extern v2 GetWindowSize();

#endif // __INPUT_H_