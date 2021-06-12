#include "Logger.h"

void LOG(const char* fmtstring, ...)
{
    va_list args;
    va_start(args, fmtstring);

    vprintf(fmtstring, args);

    va_end(args);

    puts("");
}