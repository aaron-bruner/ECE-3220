#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void * message_func( void *ptr ){
     char * message = (char *) ptr;
     printf("%s \n", message);
     sleep (5);
     return NULL;
}

int main()
{
     pthread_t thread1, thread2;
     char *message1 = "I am thread 1";
     char *message2 = "I am thread 2";
     int  ret1, ret2;

    /* Create 2 independent threads. Each will call the same function */
    /* 0 is returned on successfull return */

     ret1 = pthread_create( &thread1, NULL, message_func, (void*)message1);
       if (ret1 == 0)
           printf("Thread1 creation successfull\n");

     ret2 = pthread_create( &thread2, NULL, message_func, (void*)message2);
       if (ret2 == 0)
           printf("Thread2 creation successfull\n");


     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     printf("Thread 1 returned with: %ld\n",thread1);
     printf("Thread 2 returned with: %ld\n",thread2);
     exit(0);
}
