#include "String.h"
#include "src/core/error.h"


void StringAppend(char **dest, char *src)
{
    unsigned int destLen = (unsigned int)strlen(*dest);
    unsigned int srcLen = (unsigned int)strlen(src);

    *dest = Memory.Realloc(*dest, destLen + srcLen + 1);

    Memory.Copy(*dest + destLen, src, srcLen + 1);
}

char *NewString(char* string)
{
    unsigned int strLength = (unsigned int)strlen(string);
    char *NewString = Memory.Alloc(strLength + 1);

    Memory.Copy(NewString, string, strLength + 1);

    return NewString;   
}

unsigned int StringCountChar(char *string, char searchChar)
{
    unsigned int strLength = (unsigned int)strlen(string);
    unsigned int charCount = 0;
    for(unsigned int i = 0; i < strLength; i++)
    {
        if(string[i] == searchChar)
            charCount++;
    }
    return charCount;
}

int StringContainsString(char* string, char* searchString)
{
    unsigned int strLength = (unsigned int)strlen(string);
    unsigned int searchStringLength = (unsigned int)strlen(searchString);

    if(searchStringLength > strLength)
        return -1;

    unsigned int i;
    for(i = 0; i < strLength && (strLength - i) >= searchStringLength; i++)
    {
        if(string[i] != searchString[0])
            continue;
        
        if(Memory.Compare(string+i+1, searchString + 1, searchStringLength - 1) == 0)
        {
            return i;  
        }
    }

    return -1;
}

int StringContainsChar(char *string, char searchChar)
{
    unsigned int strLen = (unsigned int)strlen(string);

    for(unsigned int i = 0; i < strLen; i++)
        if(string[i] == searchChar) 
            return i;

    return -1;
}

void StringReplace(char *string, char searchchar, char replaceChar)
{
    unsigned int strLen = (unsigned int)strlen(string);
    for(unsigned int i = 0; i < strLen; i++)
    {
        if(string[i] == searchchar)
            string[i] = replaceChar;
    }
}

