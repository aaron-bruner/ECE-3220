#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (){
  
  puts ("Begin fork test");
  pid_t pid = fork();

  puts("End fork test");
  printf("%d\n", pid);

  return 0;
}
