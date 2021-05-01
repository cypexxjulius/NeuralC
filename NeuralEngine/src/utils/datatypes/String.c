#include "String.h"
#include "src/core/error.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>


String StringResize(String string, int count)
{
    return String(string.string + count);
}

String StringAppend(String dest, const String src)
{
    char* tempString = Memory.Realloc(dest.string, dest.length + src.length + 1);


    for(uint16_t i = 0; i < src.length + 1; i++)
    {
        tempString[i + dest.length] = src.string[i];
    }

    return String(tempString);
}


unsigned int StringCountChar(const String string, char searchChar)
{
    unsigned int charCount = 0;
    for(unsigned int i = 0; i < string.length; i++)
    {
        if(string.string[i] == searchChar)
            charCount++;
    }
    return charCount;
}

int StringContainsString(String string, const String searchString)
{
    if(string.length < searchString.length)
        return -1;

    for(unsigned int i = 0; i < string.length && (string.length - i) >= searchString.length; i++)
    {
        if(string.string [i] != searchString.string[0])
            continue;
        
        if(Memory.Compare(string.string + i + 1, searchString.string + 1, searchString.length - 1) == 0)
        {
            return i;  
        }
    }

    return -1;
}


int StringContainsChar(const String string, char searchChar)
{

    for(unsigned int i = 0; i < string.length; i++)
        if(string.string[i] == searchChar) 
            return i;

    return -1;
}

void StringReplaceChar(const String string, char searchchar, char replaceChar)
{
    for(unsigned int i = 0; i < string.length; i++)
    {
        if(string.string[i] == searchchar)
            string.string[i] = replaceChar;
    }
}


