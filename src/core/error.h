#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include "opengl.h"

#define ASSERT(boolean, message) {if(!(boolean)) { fprintf(stderr, "[ERROR] %s %d\n%s\n", __FILE__, __LINE__, (message)); exit(1); } }

void n_initError();

#endif //__ERROR_H_