#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void IncrementMemoryCount();

void DecrementMemoryCount();

unsigned int GetMemoryCount();

/*
Wrappers for Memory managment
*/

#define MemAlloc(size) malloc((size)); IncrementMemoryCount()

#define MemCalloc(count, size) calloc((count), (size)); IncrementMemoryCount()

#define MemFree(pointer) free((pointer)); DecrementMemoryCount()

#define MemRealloc(pointer, size) realloc((pointer),(size))

// Move or copy memory

#define MemCpy(dest, src, count) memcpy(dest, src, count) 

// Compare memory

#define MemCmp(dest, src, count) memcmp(dest, src, count)

/*
Creating New Object
*/

#define CreateObject(objectName) MemCalloc(1, sizeof(objectName))

