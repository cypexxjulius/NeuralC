#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>
#include <assert.h>


void InitError();

#ifdef _WIN32
#define ASSERT(boolean, message) {if(!(boolean)) { _wassert(message, __FILE__, __LINE__); } }
#else
#define ASSERT(boolean, message) {if(!(boolean)) { __assert(message, __FILE__, __LINE__); } }
#endif 
#endif //__ERROR_H_