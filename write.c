#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define SIZE 1024

void sigHandler(int sigNum);

int shmid;
void *shm;

int main() {
    key_t key = ftok(".", 1);
    shmid = shmget(key, SIZE, IPC_CREAT | 0666);
    shm = shmat(shmid, NULL, 0);

    // bool msg1: message flag for reader 1
    bool *msg1   = (bool *)shm;
    // bool msg2: message flag for reader 2
    bool *msg2   = msg1 + 1;
    // bool shutdown: shutdown flag
    bool *shutdown = msg1 + 2;
    // char buffer: message
    char *buffer = (char *)(msg1 + 3);

    *msg1 = false;
    *msg2 = false;
    *shutdown = false;

    signal(SIGINT, sigHandler);

    char input[256];

    printf("Writer ready.\n");

    while (1) {
        printf("Enter message: ");
        fgets(input, 256, stdin);

        if (strncmp(input, "quit", 4) == 0)
            raise(SIGINT);

        strcpy(buffer, input);

        // Raise both flags
        *msg1 = true;
        *msg2 = true;

        // Wait until both readers have cleared
        while (*msg1 || *msg2) { }
    }
}

void sigHandler(int sigNum) {
    if (sigNum == SIGINT) {
        printf("\nWriter: exiting gracefully.\n");

        // Readers shutdown signal
        bool *shutdown = (bool *)shm + 2;
        *shutdown = true;

        // Give time for readers to see
        sleep(3);

        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }
}