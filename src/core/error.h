#ifndef __ERROR_H_
#define __ERROR_H_

extern void printAssertMessageAndExit(char *message);

void n_initError();


#define ASSERT(boolean, message) {if(!(boolean)) { printAssertMessageAndExit(message); } }


#endif //__ERROR_H_