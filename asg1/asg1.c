// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// asg1.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_THREADS 3

// This takes in a pointer to an integer, dereferences it, multiplies it by 5 then exits.
void * thread_func (void * i) {
    int *return_value = (int *) i;
    *(return_value) *= 5;
    pthread_exit(return_value);
}

int main (int argc, char **argv) {

    if (argc != 6) { // Ensure we got the correct number of CLA
        printf("Incorrect number of command line arguments...\n\nExample: ./asg1 17 18 11 22 33\n");
        exit(1);
    }

    pthread_t tpid[NUM_THREADS];
    int tReturn[NUM_THREADS];
    for (int j = 0; j < NUM_THREADS; j++) {
        tReturn[j] = atoi(argv[j+NUM_THREADS]);
    }

    int fpid = fork();

    switch (fpid) {
        case -1: // Unable to fork
            printf("Error: Unable to fork...\n\n");
            exit(1);
        case 0: // Currently in the child function. Run the child_function.c program with arg one and two from CLA.
            execl("./child_function", "child_function", argv[1], argv[2], NULL);
        default: // Parent
            wait(NULL);

            printf("Parent process: PID %d and PPID %d\n", getpid(), getppid());

            for (int i = 0; i < NUM_THREADS; i++) { // Create 3 threads
                if(pthread_create(&tpid[i], NULL, thread_func, (void *)&tReturn[i])) {
                    printf("Error: Unable to create thread %d...\n", i++);
                    exit(1);
                }
            }
            for (int k = 0; k < NUM_THREADS; k++) { // Join 3 threads and print
                if(pthread_join(tpid[k], NULL)) {
                    printf("Error: Unable to join thread %d", k++);
                    exit(1);
                } else printf("   Thread %ld, Initial value: %d, Result is: %d\n",
                              tpid[k], atoi(argv[k+NUM_THREADS]), tReturn[k]);
            }
    }

    return 0;
}