#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#include <sys/ipc.h>  
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <stdbool.h>
#include <signal.h>
#define SIZEOFBLOCK 4096

void sigHandler (int sigNum);
bool test_and_set(bool *target);
//const char *path = "write.c";
//key_t shm_id = ftok(path, 1);


bool test_and_set(bool *target){
	bool returnValue = *target;
	*target = true;
	return returnValue;
}

int main() {
    key_t key = ftok("write.c", 0);
    int blockId = shmget(key, SIZEOFBLOCK, 0644 | IPC_CREAT);
    char *blockAddr = shmat(blockId, NULL, 0);

	do {
		bool lock = true;

		while (test_and_set(&lock));
		//critical section
		printf("Read from memory");
		printf("%s\n", blockAddr);

		lock = false;
		// remainder

	}while(true);
	shmdt(blockAddr);
    	signal (SIGINT, sigHandler);
	return 0; 
}


void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){
        printf ("Exit Gracefully\n");
        exit(0);
    }
}
