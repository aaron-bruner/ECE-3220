#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
 
void * threadFunc(void * i){
	printf("Thread Started\n");
	sleep(2);
        printf("Got a %p\n", i); 
        printf("Got a %d\n", *(int*)i);
	printf("Thread Ended\n");
}


int main(){
 
	// Thread id
	pthread_t threadId;
        int num = 17;
 
	// Create a thread that will do function threadFunc()
	int err = pthread_create(&threadId, NULL, &threadFunc, &num);

	// Check if thread is created sucessfuly
	if (err){
            printf("Thread creation failed\n");
		return err;
	}
	else
		printf("Thread Created with ID: %ld\n", threadId);
	

        printf("Waiting for thread to exit\n");

	// Wait for thread to exit
	err = pthread_join(threadId, NULL);

	// check if joining is sucessful
	if (err){
		printf( "Failed to join Thread with error %d\n", err);
		return err;
	}
 
	printf("Exiting Main\n");

	return 0;
}
