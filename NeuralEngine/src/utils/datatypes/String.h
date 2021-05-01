#ifndef __STRING_H_
#define __STRING_H_

#include "src/platform/memory.h"
#include <string.h>

// CustomString "String" Datatype struct
typedef struct String
{
    char *string;
    unsigned int length;
} String;


#define String(istring) (String) { .string = istring, .length = (unsigned int)strlen(istring) }
#define DeleteString(istring) Memory.Free(istring.string)

/*
Appends src to dest, reallocates automaticly
*/
String StringAppend(String dest, const String src);


String StringResize(String string, int count);


/*
Counts the occurence of searchChar in string
*/
unsigned int StringCountChar(const String string, char searchChar);

/*
Returns the position of searchString, 
if string does not contain searchString the function returns -1 
*/
int StringContainsString(String string, const String searchString);

/*
Returns the position of searchChar, 
if string does not contain the char the function returns -1 
*/
int StringContainsChar(String string, char searchChar);

/*
Replaces every seachchar with replacechar, 
the function only iterates once through the string
*/
void StringReplaceChar(String string, char searchchar, char replaceChar);



#endif // __STRING_H_