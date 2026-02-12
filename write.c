#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#include <sys/ipc.h>  
#include <sys/shm.h> 
#include <sys/ipc.h>

void sigHandler (int sigNum);



void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){
        printf ("Exit Gracefully\n");
        exit(0);
    }
}
