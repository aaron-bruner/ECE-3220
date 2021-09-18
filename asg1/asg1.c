#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_THREADS 3


/*  Parent Process:
 *      Create three threads:
 *          threads will be based on the function thread_func
 *              thread_func will take one integer value
 *              will take the integer and multiply it by 5
 *              return the product to parent using thread_exit
 *          thread_join in the parent
 *      Print:
 *          PID
 *          PPID
 *          Each Thread's PID
 *          Each thread's original and resulting values from the thrad
 *
 */

void * thread_func (void * i) {
    int *return_value = (int *) i;
    *(return_value) *= 5;
    pthread_exit(return_value);
}

int main (int argc, char **argv) {

    pthread_t thread_one_pid, thread_two_pid, thread_three_pid;
    int thread_one_return = atoi(argv[3]), thread_two_return = atoi(argv[4]), thread_three_return = atoi(argv[5]);

    if (argc != 6) {
        printf("Incorrect number of command line arguments...\n\nExample: ./asg1 17 18 11 22 33\n");
        exit(1);
    }

    int forkpid = fork();

    if (forkpid == -1) { // Error in fork call
        printf("Error: Unable to fork...\n\n");
        exit(1);
    }
    if (forkpid == 0) { // Child
        execl("./child_function", "child_function", argv[1], argv[2], NULL);
    }
    if (forkpid != 0 && forkpid != -1) { // Parent Function
        wait(NULL);

        printf("Parent process: PID %d and PPID %d\n", getpid(), getppid());

        if(pthread_create(&thread_one_pid, NULL, thread_func, (void *)&thread_one_return)) {
            printf("Error: Unable to create thread one...\n");
            exit(1); }
        if(pthread_create(&thread_two_pid, NULL, thread_func, (void *)&thread_two_return)) {
            printf("Error: Unable to create thread two...\n");
            exit(1); }
        if(pthread_create(&thread_three_pid, NULL, thread_func, (void *)&thread_three_return)) {
            printf("Error: Unable to create thread three...\n");
            exit(1); }

        if(pthread_join(thread_one_pid, NULL)) {
            printf("Error: Unable to join thread one...\n");
            exit(1);
        } else printf("   Thread %ld, Initial value: %d, Result is: %d\n", thread_one_pid, atoi(argv[3]), thread_one_return);
        if(pthread_join(thread_two_pid, NULL)) {
            printf("Error: Unable to join thread two...\n");
            exit(1);
        } else printf("   Thread %ld, Initial value: %d, Result is: %d\n", thread_two_pid, atoi(argv[4]), thread_two_return);
        if(pthread_join(thread_three_pid, NULL)) {
            printf("Error: Unable to join thread three...\n");
            exit(1);
        } else printf("   Thread %ld, Initial value: %d, Result is: %d\n", thread_three_pid, atoi(argv[5]), thread_three_return);
    }

    return 0;
}