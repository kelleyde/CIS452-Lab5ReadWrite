#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#include <sys/ipc.h>  
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#define SIZEOFBLOCK 4096

void sigHandler (int sigNum);
bool test_and_set(bool *target);

int main(int argc, char *argv[]) {
    key_t key = ftok("write.c", 0);
    int blockId = shmget(key, SIZEOFBLOCK, 0644 | IPC_CREAT);
    char *blockAddr = shmat(blockId, NULL, 0);

    
    char inputCheck[100];
    char input[100];
    do {
	bool lock = false;
	printf("Do you want to write to memory, yes or no \t");
	fgets(inputCheck, sizeof(inputCheck), stdin);
	if(inputCheck == "yes"){
		lock = true;
	}
	memcpy(blockAddr, &lock, SIZEOFBLOCK);
        while (test_and_set(&lock)) { } // do nothing
        /* critical section */
	while(true){
		fgets(input, sizeof(input), stdin);
		if(input == "exit"){
			break;
		}
		printf("%s", input);
		strncpy(blockAddr, input, SIZEOFBLOCK);
	}
        lock = false;
        /* remainder section */
    } while (true);

    shmdt(blockAddr);
    signal (SIGINT, sigHandler);

}

bool test_and_set (bool *target)
{
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
