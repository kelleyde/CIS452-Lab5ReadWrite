#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#include <sys/ipc.h>  
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <stdbool.h>

void sigHandler (int sigNum);
bool test_and_set(bool *target);



bool test_and_set(bool *target){
	bool returnValue = *target;
	*target = true;
	return returnValue;
}


void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){
        printf ("Exit Gracefully\n");
        exit(0);
    }
}
