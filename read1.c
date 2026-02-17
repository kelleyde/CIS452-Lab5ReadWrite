#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <signal.h>

#define SIZE 1024

void sigHandler(int sigNum);

int shmid;
void *shm;
int readerNum;

int main(int argc, char *argv[]) {
    readerNum = atoi(argv[1]);

    key_t key = ftok(".", 1);
    shmid = shmget(key, SIZE, 0666);
    shm = shmat(shmid, NULL, 0);

    bool *msg1 = (bool *)shm;
    bool *msg2 = msg1 + 1;
    bool *shutdown = msg1 + 2;
    char *buffer = (char *)(msg1 + 3);

    // Given reader will watch personal flag
    bool *myFlag = (readerNum == 1) ? msg1 : msg2;

    signal(SIGINT, sigHandler);

    printf("Reader %d ready.\n\n", readerNum);

    while (1) {
        // Wait for writer to raise personal flag
        while (!(*myFlag)) { }

        if (*shutdown) {
            raise(SIGINT);
        }

        printf("Reader %d: %s", readerNum, buffer);
        fflush(stdout);

        // Clear personal flag
        *myFlag = false;
    }
}

void sigHandler(int sigNum) {
    if (sigNum == SIGINT) {
        printf("\nReader %d: exiting gracefully.\n", readerNum);
        shmdt(shm);
        exit(0);
    }
}