#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* Child Function:
 *      Takes two integer inputs
 *      This program will compute the product of the two integers, print the original numbers
 *      and the product then exit.
 *      This program is called using the exec() family; specifically the execl() function.
 *      Finally, this function will print it's PID and the parent's PID (PPID).
 */

void main (int argc, char **argv) {
    if (argc != 3) {
        printf("Error: Did not receive correct number of CLA...\n\n");
        exit(1);
    } else {
        int CLA_1 = atoi(argv[1]);
        int CLA_2 = atoi(argv[2]);
        int total = CLA_1 * CLA_2;
        printf("Child process: PID %d and PPID %d, Initial values: %d and %d, Result; %d\n\n", getpid(), getppid(), CLA_1, CLA_2, total);
    }
    return;
}