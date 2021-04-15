#include "Timer.h"
#include "src/platform/memory.h"
#include <time.h>
#include "src/platform/platform.h"


#ifdef NEURAL_WINDOWS

    #include <Windows.h>

#endif 

static double TicksPerMiliSecond = 0;

static inline void GetCPUTime(i64* outTime)
{
#ifdef NEURAL_WINDOWS

    if(TicksPerMiliSecond == 0)
    {
        LARGE_INTEGER PerfCountFrequency;
        QueryPerformanceFrequency(&PerfCountFrequency);
        TicksPerMiliSecond = 1000.0f / (float)PerfCountFrequency.QuadPart;
    }


    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);

    *outTime = ticks.QuadPart;
#else 
    *outTime = (i64)clock();
#endif
}

void _TimerStop(TimeProfilerStruct *timer)
{
    i64 EndTime;
    GetCPUTime(&EndTime); // Stores CPU Time in EndTime;

    printf("%s %.3fms\n", timer->name, (EndTime - timer->StartTime) * TicksPerMiliSecond); // Prints the elapsed time to Screen
}

int _TimerStart(TimeProfilerStruct* timer)
{
    if(timer->StartTime != 0)
        return 0;
    
    GetCPUTime(&timer->StartTime);
    return 1;
}

