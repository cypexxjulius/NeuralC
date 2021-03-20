#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdlib.h>
#include <assert.h>


void InitError();

#define ASSERT(boolean, message) {if(!(boolean)) { _wassert(message, __FILE__, __LINE__); } }


#endif //__ERROR_H_