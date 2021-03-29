#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void incrementCount();

void decrementCount();

unsigned int getMemoryCount();

/*
Wrappers for Memory managment
*/

#define MemAlloc(size) malloc((size)); incrementCount()

#define MemCalloc(count, size) calloc((count), (size)); incrementCount()

#define MemFree(pointer) free((pointer)); decrementCount()

#define MemRealloc(pointer, size) realloc((pointer),(size))

// Move or copy memory

#define MemCpy(dest, src, count) memcpy(dest, src, count) 

// Compare memory

#define MemCmp(dest, src, count) memcmp(dest, src, count)

/*
Creating new Object
*/

#define CreateObject(objectName) MemCalloc(1, sizeof(objectName))


#endif // __MEMORY_H_