#ifndef __TIMER_H_
#define __TIMER_H_

#include "src/utils/types.h"
#include "src/core/debug.h"


typedef struct TimeProfilerStruct
{
    char *name;
    u64 StartTime;
} TimeProfilerStruct;

void _TimerStop(TimeProfilerStruct *timer);

int _TimerStart(TimeProfilerStruct *timer);

#ifdef NEURAL_DEBUG
    #define Profile(string) for(TimeProfilerStruct string##__LINE__##__FILE__ = {#string, 0}; _TimerStart(&string##__LINE__##__FILE__); _TimerStop(&string##__LINE__##__FILE__))
#else
    #define Profile(string)
#endif 
#endif // __TIMER_H_
