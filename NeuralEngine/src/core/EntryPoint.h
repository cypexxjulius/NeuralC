#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_

#include "Application.h"
#include "src/platform/memory.h"

void Start();

int main()
{   
    Start();    

    ApplicationLoop();
    return 0;
}

#endif // __ENTRYPOINT_H_