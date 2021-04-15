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
    Memory.Free(string);
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

static inline unsigned int StringCompare(char *string0, char* string1)
{
    unsigned int strLen = 0;

    for(strLen = 0; string0[strLen] != '\0' && string1[strLen] != '\0'; strLen++);

    if(strLen == 0)
        return 0;

    for(unsigned int i = 0; i < strLen; i++)
    {
        if(string0[i] != string1[i])
            return 0;
    }

    return 1;
}

#endif // __STRING_H_