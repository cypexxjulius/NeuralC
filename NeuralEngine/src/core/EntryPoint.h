#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_

#include "Application.h"
#include "src/platform/memory.h"

void Start();

int main()
{   
    Start();    

    ApplicationLoop();
    
    printf("Memory Count %u\n", GetMemoryCount());
    return (0);
}

#endif // __ENTRYPOINT_H_