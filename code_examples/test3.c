#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <string.h>

void* thread_function(void *ignoredInThisExample){
    char *a = malloc(10);
    strcpy(a,"hello world");
    pthread_t self_id = pthread_self();
    printf("In function - my id is %ld\n", self_id); 
    pthread_exit((void*)a);
}
int main()
{
    pthread_t thread_id;
    char *b;

    pthread_create (&thread_id, NULL, &thread_function, NULL);
    printf( "In main - my id is %ld\n", thread_id);    

    pthread_join(thread_id,(void**)&b); 
    /* we are reciving one pointer value so to use that we need double pointer 
    */

    printf("b is %s\n",b); 
    free(b); 
}
