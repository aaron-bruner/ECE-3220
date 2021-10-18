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
    if (a->y < b->y) return true;
    if (a->y > b->y) return false;
    return false;
}

void FIFO (void);
void SJF (void);
void RR(void);

int numElements = 0;

int main (int argc, char *argv[]) {

    if (argc != 2) { // Ensure we got the correct number of CLA
        printf("Incorrect number of command line arguments...\n\nExample: ./asg2 -fifo < in1\n");
        exit(1);
    }

    if (strcmp(argv[1], "-fifo") == 0) {
        FIFO();
    }
    else if (strcmp(argv[1], "-sjf") == 0) {
        SJF();
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

    numElements = i;
    availble_time = myVector[numElements-1].final_time+1;

    printf("FIFO scheduling results\n\ntime\tcpu\tready queue (tid/rst)\n");
    printf("--------------------------------------------\n");
    for(int k = 0; k < availble_time; k++) {
        printf("%d\t", k);
        for (int m = 0; m < numElements; m++) {
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

    printf("\n\tarrival\tservice\tcompletion\tresponse\twait\ntaskID\ttime\ttime\ttime\t\ttime\t\ttime\n");
    for(int q = 0; q < 60; q++) { printf("-"); }
    printf("\n");

    for (i = 0; i < numElements; i++) {
        int response_time = myVector[i].num_tasks + (myVector[i].end_queue - myVector[i].begin_queue)+1;
        int wait = myVector[i].end_queue - myVector[i].begin_queue+1;
        if (myVector[i].end_queue == -1){
            wait = 0;
            response_time = myVector[i].num_tasks;
        }
        printf("%c\t%d\t%d\t%d\t\t%d\t\t%d\n", myVector[i].taskID, myVector[i].orginal_time, myVector[i].num_tasks, myVector[i].final_time+1, response_time, wait);
    }

    printf("\nservice wait\ntime\ttime\n");
    printf("-----------------------------------------------------------\n");
    int j = 0;
    for (int i = numElements-1; i >= 0; i--) {
        int wait = myVector[i].end_queue - myVector[i].begin_queue+1;
        if (myVector[i].end_queue == -1){
            wait = 0;
        }
        printf("%d\t%d\n", myVector[i].num_tasks, wait);
    }

}
void SJF(void) {
    task myVector1[26];
    task2 myVector2[26];

    char taskID = '@';
    int x1, x2, x3 = 1, i = 0, temp = 0;

    while (scanf("%d", &x1) != EOF) {
        scanf("%d", &x2);
        myVector2[i].x = x1;
        myVector2[i].y = x2;
        myVector2[i].placement = x3;
        x3 += 1;
        i++;
    }

    numElements = i;

    for (int i = 0; i < numElements; i++) {
        for (int j = 1; j < numElements; j++) {
            if (myVector2[i].y < myVector2[j].y){
                temp = myVector2[i].placement;
                myVector2[i].placement = myVector2[j].placement;
                myVector2[j].placement = temp;
                temp = myVector2[i].x;
                myVector2[i].x = myVector2[j].x;
                myVector2[j].x = temp;
                temp = myVector2[i].y;
                myVector2[i].y = myVector2[j].y;
                myVector2[j].y = temp;
            }
        }
    }

    int availble_time = 0;
    int u = 0;
    for (int p = 0; p < numElements; p++) {
        myVector1[p].taskID = taskID + myVector2[p].placement;
        myVector1[p].orginal_time = myVector2[p].x;
        myVector1[p].arrival_time = myVector2[p].x;
        myVector1[p].final_time = myVector2[p].x+myVector2[p].y-1;
        myVector1[p].num_tasks = myVector2[p].y;
        myVector1[p].task_counter = myVector2[p].y;
        myVector1[p].begin_queue = -1;
        myVector1[p].end_queue = -1;
        for (int i = 0; i < u; i++) {
            if (myVector1[p].arrival_time <= myVector1[i].final_time) {
                myVector1[p].begin_queue = myVector1[p].arrival_time;
                myVector1[p].end_queue = availble_time - 1;
                myVector1[p].arrival_time = availble_time;
                myVector1[p].final_time = availble_time + myVector1[p].num_tasks - 1;
            }
        }
        u++;
        availble_time = myVector1[p].final_time + 1;
        //printf("myVector1[%d] - taskid(%c) org_tim(%d) arr_tim(%d) fin_tim(%d) num_tsk(%d) tsk_ctr(%d) beg_Q(%d) endQ(%d)\n", p, myVector1[p].taskID, myVector1[p].orginal_time, myVector1[p].arrival_time, myVector1[p].final_time, myVector1[p].num_tasks, myVector1[p].task_counter, myVector1[p].begin_queue, myVector1[p].end_queue);
    }
    availble_time = myVector1[numElements -1].final_time + 1;
    //printf("%d\n", availble_time);

    printf("SJF(preemptive) scheduling results\n\ntime\tcpu\tready\tqueue (tid/rst)\n");
    printf("--------------------------------------------\n");

    for (int i = 0; i < availble_time; i++) {
        printf("%d\t", i);
        for (int j = 0; j < numElements; j++) {
            if (i >= myVector1[j].arrival_time && i <= myVector1[j].final_time) {
                printf("%c%d    ", myVector1[j].taskID, myVector1[j].task_counter);
                myVector1[j].task_counter -= 1;
            }
            else if (i >= myVector1[j].begin_queue && i <= myVector1[j].end_queue && myVector1[j].begin_queue != 0) {
                printf("\t%c%d", myVector1[j].taskID, myVector1[j].task_counter);
            }
        }
        printf("\n");
    }

    printf("\n\tarrival\tservice\tcompletion\tresponse\twait\ntaskID\ttime\ttime\ttime\t\ttime\t\ttime\n");
    for(int q = 0; q < 60; q++) { printf("-"); }
    printf("\n");

    for (i = 0; i < numElements; i++) {
        int response_time = myVector1[i].num_tasks + (myVector1[i].end_queue - myVector1[i].begin_queue)+1;
        int wait = myVector1[i].end_queue - myVector1[i].begin_queue+1;
        if (myVector1[i].end_queue == -1){
            wait = 0;
            response_time = myVector1[i].num_tasks;
        }
        printf("%c\t%d\t%d\t%d\t\t%d\t\t%d\n", myVector1[i].taskID, myVector1[i].orginal_time, myVector1[i].num_tasks, myVector1[i].final_time+1, response_time, wait);
    }

    printf("\nservice wait\ntime\ttime\n");
    printf("-----------------------------------------------------------\n");
    int j = 0;
    for (int i = 0; i < numElements; i++) {
        int wait = myVector1[i].end_queue - myVector1[i].begin_queue+1;
        if (myVector1[i].end_queue == -1){
            wait = 0;
        }
        printf("%d\t%d\n", myVector1[i].num_tasks, wait);
    }

}
void RR(void) {}
























