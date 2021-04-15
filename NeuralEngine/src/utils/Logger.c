#include "Logger.h"
#include <stdarg.h>
#include <string.h>

struct LogFormatParse 
{
    char *format;
    char *printfFormat;

    unsigned int lengthFormat, lengthPrintfFormat;
};

#define LogFormatParse(iformat, iprintfFormat) { .format=iformat, .printfFormat=iprintfFormat, .lengthFormat=sizeof(iformat) - 1, .lengthPrintfFormat=sizeof(iprintfFormat) - 1 }
const struct LogFormatParse stdParseArray[] = 
{
    LogFormatParse( "{int}", "%i"),
    LogFormatParse( "{float}", "%f"),
    LogFormatParse( "{char *}", "%s"),
    LogFormatParse( "{char}", "%c"),
    LogFormatParse( "{unsigned int}", "%u"),
    LogFormatParse( "{hex}", "%x")
};

const unsigned int stdParseArrayLength = sizeof(stdParseArray) / sizeof(struct LogFormatParse); 


void Log(char *logMessage, ...)
{
    va_list args;

    va_start(args, logMessage);

    unsigned int strLen = (unsigned int)strlen(logMessage);

    for(unsigned int i = 0; i < strLen; i++)
    {
        if(logMessage[i] == '{')
        {
            //fstringCompare();
        }

    }
}