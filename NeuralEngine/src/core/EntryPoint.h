#pragma once

#include "Application.h"


Layer* SetGameLayer();


int main()
{   
    CreateApplication("Test Layer");

    ApplicationLayerAdd(SetGameLayer());    

    ApplicationLoop();
    return 0;
}
