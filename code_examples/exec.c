// to run complie both count.c and exec.c
// gcc count.c -o count
// gcc exec.c -o exec
// to run: ./exec 5

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main (int argc, char * argv[]){

    if (fork() == 0) /* in child process */
                execl ("./count", "count", argv[1], NULL);
    else   { /* in parent process */
        printf ("In parent process \n"); 
    } 

    wait (NULL);  
    printf ("In parent, child complete\n");

   return 0;
}

