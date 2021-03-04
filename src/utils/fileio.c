#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "../platform/memory.h"
#include "../core/error.h"

extern char* n_readFile(char *filepath)
{
    FILE* fp = fopen(filepath, "rb");
    if(!fp)
    {
        puts(filepath);
        ASSERT(0, "File does not exist");
    }

    fseek(fp, 0, SEEK_END); 
    unsigned int length = ftell(fp);
    rewind(fp);

    char *file = nl_calloc(length, sizeof(char));

    if(!file)
    {
        ASSERT(0, "Memory allocation failed");
    }

    fread(file, length, sizeof(char), fp);
    fclose(fp);
    return file;
}