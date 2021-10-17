// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// asg2.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_TASK = 26;

//struct task{
//    int task_id, /* alphabetic tid can be obtained as 'A'+(task_counter++) */
//    arrival_time,
//    service_time,
//    remaining_time,
//    completion_time,
//    response_time,
//    wait_time;
//    struct task *next;
//};

typedef struct task_struct {
    char taskID;
    int orginal_time;
    int arrival_time;
    int final_time;
    int num_tasks;
    int task_counter;
    int begin_queue;
    int end_queue;
}task;

typedef struct task2_struct {
    int x;
    int y;
    int placement;
}task2;

bool task2_sort (const task2 *a, const task2 *b) {
    if (a->x < b->x) return true;
    if (a->x > b->x) return false;
    if (a->y< b->y) return true;
    if (a->y > b->y) return false;
    return false;
}

void FIFO (void);
void SJF (void);
void RR(void);

int main (int argc, char *argv[]) {

    if (argc != 2) { // Ensure we got the correct number of CLA
        printf("Incorrect number of command line arguments...\n\nExample: ./asg2 -fifo < in1\n");
        exit(1);
    }

    if (strcmp(argv[1], "-fifo") == 0) {
        FIFO();
    }
    else if (strcmp(argv[1], "-sjf") == 0) {
        //SJF();
    }
    else if (strcmp(argv[1], "-rr") == 0) {
        //RR();
    }
    else {
        printf("Error: Invalid command line argument...\n");
        exit(1);
    }

    return 0;
}


void FIFO(void) {
    int x1,x2, i = 0;
    char taskID = 'A';
    int availble_time =0;
    task myVector[26];
    while (scanf("%d", &x1) != EOF){
        scanf("%d", &x2);
        myVector[i].taskID = taskID;
        myVector[i].orginal_time= x1;
        myVector[i].arrival_time = x1;
        myVector[i].final_time= x1+x2-1;
        myVector[i].num_tasks = x2;
        myVector[i].task_counter=x2;
        myVector[i].begin_queue = -1;
        myVector[i].end_queue = -1;
        for(int j = 0; j < i; j++){
            if (myVector[i].arrival_time <= myVector[j].final_time){
                myVector[i].begin_queue =myVector[i].arrival_time;
                myVector[i].end_queue = availble_time-1;
                myVector[i].arrival_time = availble_time;
                myVector[i].final_time = availble_time + myVector[i].num_tasks - 1;
            }
        }
        availble_time = myVector[i].final_time+1;
        taskID++;
        i++;
    }

    availble_time = myVector[i-1].final_time+1;
    for (int p = 0; p < i; p++) {
        printf("myVector[%d].qbegining: %d qend: %d\n", p, myVector[p].begin_queue, myVector[p].end_queue);
    }
//printing first table
    printf("FIFO scheduling results\n\ntime\tcpu\tready queue (tid/rst)\n");
    printf("--------------------------------------------\n");
    for(int k = 0; k < availble_time; k++) {
        printf("%d\t", k);
        for (int m = 0; m < i; m++) {
            if(k >= myVector[m].arrival_time && k <= myVector[m].final_time) {
                printf("%c%d", myVector[m].taskID, myVector[m].task_counter);
                myVector[m].task_counter -= 1;
            }
            else if( k >= myVector[m].begin_queue && k <= myVector[m].end_queue) {
                printf("\t%c%d", myVector[m].taskID, myVector[m].task_counter);
            } else if (myVector[m].final_time > k && myVector[m].begin_queue >= k){
                //printf("--");
            }
        }
        printf("\n");
    }
}
void SJF(void) {}
void RR(void) {}
























