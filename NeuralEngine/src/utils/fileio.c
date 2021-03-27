#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "src/platform/memory.h"
#include "src/core/error.h"

extern char* n_readFile(char *filepath)
{
    FILE* fp = fopen(filepath, "rb");
    if(!fp)
    {
        puts(filepath);
        ASSERT(0, "File does not exist");
    }

    fseek(fp, 0, SEEK_END);
    unsigned int fileLength = ftell(fp); 
    fseek(fp, 0, SEEK_SET);

    char *file = MemCalloc(fileLength + 1, sizeof(char));

    if(!file)
    {
        ASSERT(0, "Memory allocation failed");
    }

    fread(file, fileLength, sizeof(char), fp);
    fclose(fp);
    return file;
}