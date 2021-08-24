#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[]){
   int i, limit;
   if (argc == 1){
      puts("Error");
      exit(1);   
   }
   limit = atoi(argv[1]);
   for (i = 0; i <= limit; i++)
     printf (" we have %d\n", i);
}
