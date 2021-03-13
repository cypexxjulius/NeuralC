#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>

extern void printAssertMessageAndExit(char *message, char *file, int line);

void InitError();


#define ASSERT(boolean, message) {if(!(boolean)) { printAssertMessageAndExit(message, __FILE__, __LINE__); } }


#endif //__ERROR_H_