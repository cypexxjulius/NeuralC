#ifndef __PLATFORM_H_
#define __PLATFORM_H_

enum PLATTFORM
{
    NOT_KNOWN,
    WINDOWS,
    LINUX,
    MACOS
};

#ifdef _WIN32

    #define NEURAL_PLATFORM WINDOWS
    #define NEURAL_WINDOWS

#endif 

#ifdef __unix__

    #define NEURAL_PLATFORM LINUX
    #define NEURAL_LINUX

#endif 

#ifdef __APPLE__

    #define NEURAL_PLATFORM MACOS
    #define NEURAL_MACOS

#endif 


#endif // __PLATFORM_H_