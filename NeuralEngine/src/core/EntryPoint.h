#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_

#include "Application.h"
#include "src/platform/memory.h"

void CreateApplication();

int main()
{   
    CreateApplication();    

    ApplicationLoop();
    return 0;
}

#endif // __ENTRYPOINT_H_