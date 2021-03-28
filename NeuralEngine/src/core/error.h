#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>
#include <Assert.h>


void InitError();


#ifdef _WIN32
#define Assert(boolean, message) { if((boolean)) { _wassert(message, __FILE__, __LINE__); } }

#else
#define Assert(boolean, message) {if((boolean)) { __Assert(message, __FILE__, __LINE__); } }
#endif


#define CoreWarn(messageFormat, ...) fprintf(stderr, messageFormat, __VA_ARGS__)
#endif //__ERROR_H_