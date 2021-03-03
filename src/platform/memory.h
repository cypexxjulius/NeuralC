#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../n_types.h"

#include <stdlib.h>
#include <stdio.h>

void incrementCount();

void decrementCount();

i16 getMemoryCount();

#define nl_malloc(size) malloc((size)); incrementCount();
#define nl_calloc(count, size) calloc((count), (size)); incrementCount();;
#define nl_free(pointer) { free((pointer)); decrementCount();}

#define nl_realloc(pointer, size) realloc((pointer),(size));


#endif // __MEMORY_H_