#pragma once

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
