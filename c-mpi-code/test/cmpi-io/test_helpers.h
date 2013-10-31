
// #include <stdio.h> 

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <node.h>

/**
   Print debugging header using machine-specific information. 
*/
void print_header(void);

char* useless_data(int size); 

void wait_for_notification(void); 
void notify_next(void); 

#endif
