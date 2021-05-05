#include "Logger.h"
#include "src/core/error.h"

#define MAX_DEBUG_MESSAGE_LENGTH 4096

void nl_printdb(const char * string, ...)
{
    va_list args;
    va_start(args, string);

    unsigned int Strlen = (unsigned int)strlen(string);

    // Checking for the stringlen to be within a certain range(MAX_DEBUG_MESSAGE_LENGTH)
    Assert(Strlen > MAX_DEBUG_MESSAGE_LENGTH, "Debug message to long");

    // Creating the buffer
    char buffer[MAX_DEBUG_MESSAGE_LENGTH + sizeof("[DEBUG] ") + sizeof('\n')] = "[DEBUG] ";
    
    // Inserting the string into the buffer
    memcpy(buffer + sizeof("[DEBUG] ") - 1, string, Strlen);
    
    // Appending the endline char
    static const char endl[] = "\n";
    memcpy(buffer + sizeof("[DEBUG] ") - 1+ Strlen, endl, 1);
    

    // Printing the message to the screen
    vfprintf(stdout, buffer, args);

    va_end(args);
}