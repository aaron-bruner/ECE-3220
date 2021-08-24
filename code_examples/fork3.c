#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (){
  
int pid = fork();

  if (pid == 0) { // child process
      printf("I am child process with id: %d\n", getpid());
  } else {   
    // parent process
    printf("Child's PID is %d\n", pid);
    printf("Parent's PID is %d\n", getpid()); 
    printf("Parent's parent's PID is %d\n", getppid()); 
  }

  return 0;
}
