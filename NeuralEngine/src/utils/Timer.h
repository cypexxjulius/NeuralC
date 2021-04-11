#ifndef __TIMER_H_
#define __TIMER_H_

void _TimerStop();

int _TimerStart(char *string);

#define Profile(string) for(; _TimerStart(string); _TimerStop())

#endif // __TIMER_H_