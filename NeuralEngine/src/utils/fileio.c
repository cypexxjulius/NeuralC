#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "src/platform/memory.h"
#include "src/core/error.h"

extern String ReadStringFromFile(char *filepath)
{
    FILE* fp = fopen(filepath, "rb");           // Opens the File

    if(!fp)
    {   
        // Display error message
        char errorMessage[300];
        snprintf(errorMessage, 300, "Could not open file : %s", filepath); 
        Assert(1, errorMessage);                        
    }

    fseek(fp, 0, SEEK_END);                     // Seeking to the EOF
    unsigned int fileLength = ftell(fp);        // Reading the position of the Cursor (last position in File = Length of File)
    rewind(fp);                                 // Resetting the Cursor to the Beginning

    char *file = Memory.Alloc(fileLength + 1); // Reserving space for the filecontent in a buffer

    fread(file, fileLength, sizeof(char), fp);  // Reading the filecontent into the Buffer
    file[fileLength] = 0;                       // Appending End of File


    fclose(fp);                                 // Closing the file
    return (String)
    {
        .length = fileLength,
        .string = file
    }; // Return the Buffer
}