#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "src/utils/types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void incrementCount();

void decrementCount();

i16 getMemoryCount();

/*
Wrappers for Memory managment
*/

#define MemAlloc(size) malloc((size)); incrementCount()

#define MemCalloc(count, size) calloc((count), (size)); incrementCount()

#define MemFree(pointer) free((pointer)); decrementCount()

#define MemRealloc(pointer, size) realloc((pointer),(size))

// Move or copy memory

#define MemCpy(dest, src, count) memcpy(dest, src, count) 

/*
Creating new Object
*/

#define CreateObject(objectName) MemCalloc(1, sizeof(objectName))


#endif // __MEMORY_H_