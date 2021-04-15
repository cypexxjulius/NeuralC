#ifndef __TIMER_H_
#define __TIMER_H_

#include "src/utils/types.h"


typedef struct TimeProfilerStruct
{
    char *name;
    u64 StartTime;
} TimeProfilerStruct;

void _TimerStop(TimeProfilerStruct *timer);

int _TimerStart(TimeProfilerStruct *timer);

#define Profile(string) for(TimeProfilerStruct string##__LINE__##__FILE__ = {#string, 0}; _TimerStart(&string##__LINE__##__FILE__); _TimerStop(&string##__LINE__##__FILE__))

#endif // __TIMER_H_