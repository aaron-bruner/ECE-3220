#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<signal.h> 
#include<sys/wait.h> 

pid_t cpid = -1; int main() {

  cpid = fork(); if (cpid > 0) {
    // Parent process printf("Printing from parent process\n"); 
    while(1){}
  }else if (cpid == 0){
    // Child process printf("Printing from child process\n"); 
  pid_t ppid = getppid(); 
  printf("Parent ID: %ld\n", (long) ppid); 
  kill(ppid, SIGKILL); 
sleep(3); 
printf("Parent Dead :( \n"); 
ppid = getppid(); 
    printf("Parent ID: %ld\n", (long) ppid);
  } return 0; }
