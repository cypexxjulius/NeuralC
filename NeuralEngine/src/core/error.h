#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>
#include <assert.h>


void InitError();

void __ASSERT(char *message, char *file, unsigned int line);


#define Assert(boolean, message) if((boolean)) __ASSERT(message, __FILE__, __LINE__)


#define CoreWarn(...) fprintf(stderr, __VA_ARGS__)
#endif //__ERROR_H_