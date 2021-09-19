// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// child_function.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void main (int argc, char **argv) { // This is the most self explanatory code... I don't know what to put
    if (argc != 3) {
        printf("Error: Did not receive correct number of CLA...\n\n");
        exit(1);
    } else {
        int CLA_1 = atoi(argv[1]);
        int CLA_2 = atoi(argv[2]);
        int total = CLA_1 * CLA_2;
        printf("Child process: PID %d and PPID %d, Initial values: %d and %d, Result; %d\n\n",
               getpid(), getppid(), CLA_1, CLA_2, total);
    }
    return;
}