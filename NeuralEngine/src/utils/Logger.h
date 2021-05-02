#ifndef __LOGGER_H_
#define __LOGGER_H_

#include "types.h"
#include <stdarg.h>

extern void nl_printdb(const char *string, ...);

#define DebugPrint(string, ...) nl_printdb(string, __VA_ARGS__)




#endif // __LOGGER_H_