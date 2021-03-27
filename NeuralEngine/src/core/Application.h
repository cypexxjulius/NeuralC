#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Layer.h"

typedef struct Application
{   
    Layer* gameLayer;
    char *name;
} Application;



Layer* SetGameLayer();

#endif // __APPLICATION_H_