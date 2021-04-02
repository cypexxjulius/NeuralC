#ifndef __ENTRYPOINT_H_
#define __ENTRYPOINT_H_

#include "Application.h"


Layer* SetGameLayer();


int main()
{   
    CreateApplication("Test Layer");

    ApplicationLayerAdd(SetGameLayer());    

    ApplicationLoop();

    
    printf("Memory Count %d\n", GetMemoryCount());
    return 0;
}

#endif // __ENTRYPOINT_H_