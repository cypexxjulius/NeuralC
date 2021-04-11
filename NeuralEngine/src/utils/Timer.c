#include "Timer.h"
#include "src/platform/memory.h"
#include <time.h>

static double startTime = 0;
static int isactive = 0;
static char *ActiveName = NULL;

void _TimerStop()
{
    double now = clock();

    printf("%s %f ms\n", ActiveName, ((float)(now - startTime) / CLOCKS_PER_SEC) * 1000.0f);
    isactive = 1;
}

int _TimerStart(char *string)
{
    if(isactive)
        return isactive = 0;
    
    ActiveName = string;

    startTime = clock();

    return 1;
}

