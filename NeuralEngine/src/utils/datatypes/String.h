#ifndef __STRING_H_
#define __STRING_H_

#include "src/platform/memory.h"

/*
Appends src to dest, reallocates automaticly
*/
void StringAppend(char **dest, char *src);

/*
Creates New Heap allocated String copy of string
*/
char *NewString(char* string);

/*
Frees string, only for Heap allocated Strings
*/
static inline void DeleteString(char* string)
{
    MemFree(string);
}

/*
Counts the occurence of searchChar in string
*/
unsigned int StringCountChar(char *string, char searchChar);

/*
Returns the position of searchString, 
if string does not contain searchString the function returns -1 
*/
int StringContainsString(char* string, char* searchString);

/*
Returns the position of searchChar, 
if string does not contain the char the function returns -1 
*/
int StringContainsChar(char *string, char searchChar);

/*
Replaces every seachchar with replacechar, 
the function only iterates once through the string
*/
void StringReplace(char *string, char searchchar, char replaceChar);

#endif // __STRING_H_