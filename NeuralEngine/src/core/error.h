#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>
#include <assert.h>

extern void InitError();

#define Assert(boolean, string) assert(!(boolean))

#define CoreWarn(...) fprintf(stderr, __VA_ARGS__)

#endif // __ERROR_H_