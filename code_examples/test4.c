#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS     5

void *PrintHello(void *threadid){
   pthread_exit(threadid);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int ret;
   int t;
   void * retval;

   for(t = 0; t < NUM_THREADS; t++){
      printf("In main: creating thread %d\n", t);
      ret = pthread_create(&threads[t], NULL, PrintHello, (void *)&t);
   }


   //need to join threads here
   for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
        printf("Thread %ld joined\n", threads[t]);
   }
return 0;
}
