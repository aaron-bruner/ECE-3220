// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// asg2.c

// RR does not work /:

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

    // Below are used in RR
    int service_time;
    int completion_time;
    int remaining_time;
    int response_time;
    int wait_time;
}task;

typedef struct task2_struct {
    int x;
    int y;
    int placement;
}task2;

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
        RR();
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
    int availableTime =0;
    task taskList[26];
    while (scanf("%d", &x1) != EOF) { // Read in all of the values from the file
        scanf("%d", &x2);
        taskList[i].taskID = taskID;
        taskList[i].orginal_time= x1;
        taskList[i].arrival_time = x1;
        taskList[i].final_time= x1 + x2 - 1;
        taskList[i].num_tasks = x2;
        taskList[i].task_counter= x2;
        taskList[i].begin_queue = -1;
        taskList[i].end_queue = -1;
        for(int j = 0; j < i; j++){
            if (taskList[i].arrival_time <= taskList[j].final_time) { // Found more than one program with the same arrival time
                taskList[i].begin_queue = taskList[i].arrival_time;
                taskList[i].end_queue = availableTime-1;
                taskList[i].arrival_time = availableTime;
                taskList[i].final_time = availableTime + taskList[i].num_tasks - 1;
            }
        }
        availableTime = taskList[i].final_time + 1;
        taskID++;
        i++;
    }

    numElements = i;
    availableTime = taskList[numElements-1].final_time+1;

    printf("FIFO scheduling results\n\ntime\tcpu\tready queue (tid/rst)\n");
    printf("--------------------------------------------\n");
    for(int k = 0; k < availableTime; k++) {
        printf("%d\t", k);
        for (int m = 0; m < numElements; m++) {
            if(k >= taskList[m].arrival_time && k <= taskList[m].final_time) { // If it's ready to be put on CPU
                printf("%c%d", taskList[m].taskID, taskList[m].task_counter);
                taskList[m].task_counter -= 1;
            }
            else if( k >= taskList[m].begin_queue && k <= taskList[m].end_queue) { // If there are elements in the queue
                printf("\t%c%d", taskList[m].taskID, taskList[m].task_counter);
            } else if (taskList[m].final_time > k && taskList[m].begin_queue >= k){
                //printf("--");
            }
        }
        printf("\n");
    }

    printf("\n\tarrival\tservice\tcompletion\tresponse\twait\ntaskID\ttime\ttime\ttime\t\ttime\t\ttime\n");
    for(int q = 0; q < 60; q++) { printf("-"); }
    printf("\n");

    for (i = 0; i < numElements; i++) {
        int response_time = taskList[i].num_tasks + (taskList[i].end_queue - taskList[i].begin_queue) + 1;
        int wait = taskList[i].end_queue - taskList[i].begin_queue + 1;
        if (taskList[i].end_queue == -1){
            wait = 0;
            response_time = taskList[i].num_tasks;
        }
        printf("%c\t%d\t%d\t%d\t\t%d\t\t%d\n", taskList[i].taskID, taskList[i].orginal_time, taskList[i].num_tasks, taskList[i].final_time+1, response_time, wait);
    }

    printf("\nservice wait\ntime\ttime\n");
    printf("-----------------------------------------------------------\n");
    int j = 0;
    for (int i = numElements-1; i >= 0; i--) {
        int wait = taskList[i].end_queue - taskList[i].begin_queue+1;
        if (taskList[i].end_queue == -1){
            wait = 0;
        }
        printf("%d\t%d\n", taskList[i].num_tasks, wait);
    }

}
void SJF(void) {
    task taskList1[26];
    task2 taskList2[26];

    char taskID = '@';
    int x1, x2, x3 = 1, i = 0, temp = 0;

    while (scanf("%d", &x1) != EOF) { // Read in our values
        scanf("%d", &x2);
        taskList2[i].x = x1;
        taskList2[i].y = x2;
        taskList2[i].placement = x3;
        x3 += 1;
        i++;
    }

    numElements = i;

    for (int i = 0; i < numElements; i++) {
        for (int j = 1; j < numElements; j++) {
            if (taskList2[i].y < taskList2[j].y){ // Sort based on runtime
                temp = taskList2[i].placement;
                taskList2[i].placement = taskList2[j].placement;
                taskList2[j].placement = temp;
                temp = taskList2[i].x;
                taskList2[i].x = taskList2[j].x;
                taskList2[j].x = temp;
                temp = taskList2[i].y;
                taskList2[i].y = taskList2[j].y;
                taskList2[j].y = temp;
            }
        }
    }

    int availableTime = 0;
    int u = 0;
    for (int p = 0; p < numElements; p++) { // Figure out when things need to be printed
        taskList1[p].taskID = taskID + taskList2[p].placement;
        taskList1[p].orginal_time = taskList2[p].x;
        taskList1[p].arrival_time = taskList2[p].x;
        taskList1[p].final_time = taskList2[p].x+taskList2[p].y-1;
        taskList1[p].num_tasks = taskList2[p].y;
        taskList1[p].task_counter = taskList2[p].y;
        taskList1[p].begin_queue = -1;
        taskList1[p].end_queue = -1;
        for (int i = 0; i < u; i++) {
            if (taskList1[p].arrival_time <= taskList1[i].final_time) {
                taskList1[p].begin_queue = taskList1[p].arrival_time;
                taskList1[p].end_queue = availableTime - 1;
                taskList1[p].arrival_time = availableTime;
                taskList1[p].final_time = availableTime + taskList1[p].num_tasks - 1;
            }
        }
        u++;
        availableTime = taskList1[p].final_time + 1;
        //printf("taskList1[%d] - taskid(%c) org_tim(%d) arr_tim(%d) fin_tim(%d) num_tsk(%d) tsk_ctr(%d) beg_Q(%d) endQ(%d)\n", p, taskList1[p].taskID,
        // taskList1[p].orginal_time, taskList1[p].arrival_time, taskList1[p].final_time, taskList1[p].num_tasks, taskList1[p].task_counter, taskList1[p].begin_queue, taskList1[p].end_queue);
    }
    availableTime = taskList1[numElements -1].final_time + 1;
    //printf("%d\n", availableTime);

    // Here we will print all the data following the assignment examples.

    printf("SJF(preemptive) scheduling results\n\ntime\tcpu\tready\tqueue (tid/rst)\n");
    printf("--------------------------------------------\n");

    for (int i = 0; i < availableTime; i++) {
        printf("%d\t", i);
        for (int j = 0; j < numElements; j++) {
            if (i >= taskList1[j].arrival_time && i <= taskList1[j].final_time) {
                printf("%c%d    ", taskList1[j].taskID, taskList1[j].task_counter);
                taskList1[j].task_counter -= 1;
            }
            else if (i >= taskList1[j].begin_queue && i <= taskList1[j].end_queue && taskList1[j].begin_queue != 0) {
                printf("\t%c%d", taskList1[j].taskID, taskList1[j].task_counter);
            }
        }
        printf("\n");
    }

    printf("\n\tarrival\tservice\tcompletion\tresponse\twait\ntaskID\ttime\ttime\ttime\t\ttime\t\ttime\n");
    for(int q = 0; q < 60; q++) { printf("-"); }
    printf("\n");

    for (i = 0; i < numElements; i++) { // If we have elements in the queue then print them
        int response_time = taskList1[i].num_tasks + (taskList1[i].end_queue - taskList1[i].begin_queue)+1;
        int wait = taskList1[i].end_queue - taskList1[i].begin_queue+1;
        if (taskList1[i].end_queue == -1){
            wait = 0;
            response_time = taskList1[i].num_tasks;
        }
        printf("%c\t%d\t%d\t%d\t\t%d\t\t%d\n", taskList1[i].taskID, taskList1[i].orginal_time, taskList1[i].num_tasks, taskList1[i].final_time+1, response_time, wait);
    }

    printf("\nservice wait\ntime\ttime\n");
    printf("-----------------------------------------------------------\n");
    int j = 0;
    for (int i = 0; i < numElements; i++) {
        int wait = taskList1[i].end_queue - taskList1[i].begin_queue+1;
        if (taskList1[i].end_queue == -1){
            wait = 0;
        }
        printf("%d\t%d\n", taskList1[i].num_tasks, wait);
    }

}
void RR(void) {

    // RR does not work. I can't it to share the load. It pretty much acts like SJF.

    task taskList[26];

    printf("RR scheduling results (time slice is 1)\n\ntime  cpu  ready queue (tid/rst)");
    printf("\n----------------------------------\n");
    int time = 0, i = 0, x1, x2;
    char taskID = '@';
    int taskCounter = numElements = 0;

    while (scanf("%d", &x1) != EOF && i < 26) { // Read in all of the values from the file
        scanf("%d", &x2);
        taskList[i].taskID = taskID;
        taskList[i].orginal_time= x1;
        taskList[i].arrival_time = x1;
        taskList[i].final_time= x1 + x2 - 1;
        taskList[i].num_tasks = x2;
        taskList[i].task_counter= x2;
        taskList[i].begin_queue = -1;
        taskList[i].end_queue = -1;
        taskList[i].remaining_time = x2;
        taskList[i].wait_time = 0;
//        for(int j = 0; j < i; j++){
//            if (taskList[i].arrival_time <= taskList[j].final_time) { // Found more than one program with the same arrival time
//                taskList[i].begin_queue = taskList[i].arrival_time;
//                taskList[i].end_queue = availableTime-1;
//                taskList[i].arrival_time = availableTime;
//                taskList[i].final_time = availableTime + taskList[i].num_tasks - 1;
//            }
//        }
        taskID++;
        i++;
    }

    //availableTime = taskList[numElements-1].final_time + 1;
    //printf("%d\n\n", taskList[2].final_time);
    taskCounter = i;
    numElements = i;
    int check = 0;

    // I tried to use the idea of 'am I running and who's next' but couldn't get them to switch if there's one next

    while(taskCounter > 0) {
        int u = 0;
        int running = 0;
        for(int i = 0; i <= numElements; i++){ // find something to run
            if(taskList[i].remaining_time > 0 && taskList[i].arrival_time <= time) {
                running = 1;
            }
        }
        if(running == 0) { // nothing to run so print time
            printf(" %2d        --\n", time); time++;
        } else {
            int found = 0;
            while(found == 0) { // I am running, who's next
                if (u == numElements) u = 0; // if we start at element 1 then we don't want to skip 0
                if(taskList[u].remaining_time > 0 && taskList[u].arrival_time <= time) found = 1; // are you running with me?
                u++;
            }
            u--;
            printf(" %2d   %c%d   ", time, taskList[u].taskID+1, taskList[u].remaining_time);
            taskList[u].remaining_time -= 1; // since we printed it once we reduce it's remaining time on the cpu

            if(taskList[u].remaining_time == 0) { // I finished running so record my data for the table
                taskCounter--;
                taskList[u].completion_time = time + 1;
                taskList[u].response_time = taskList[u].completion_time - taskList[u].arrival_time - 1;
            }

            int queue = 0; // this queue is more complex than our structure's two variables
            if (u+1 == numElements) u=0; // if we're at the end
            else u++;
            for(i = u; i < numElements; i++) { // we don't want to over checks
                if(taskList[i].remaining_time > 0 && taskList[i].arrival_time <= time) {
                    if(queue == 0) { // one thing ready to be on cpu
                        printf("%c%d", taskList[i].taskID, taskList[i].remaining_time);
                    } else { // more than one thing ready to be on cpu
                        printf(", %c%d", taskList[i].taskID, taskList[i].remaining_time);
                    }
                    taskList[i].wait_time++; // since I am not on cpu count how long I have to wait
                    queue++;
                }
                //if (u == taskCounter) u = 0; // go back to the first task
            }
            if(queue == 0) { // queue empty
                printf("--\n");
            } else { // someone was printed
                printf("\n");
            }
            time++; // cpu counter
        }
    }

    // Printing just like we did for FIFO and SJF

    printf("\n\tarrival\tservice\tcompletion\tresponse\twait\ntaskID\ttime\ttime\ttime\t\ttime\t\ttime\n");
    for(int q = 0; q < 60; q++) { printf("-"); }
    printf("\n");

    for (i = 0; i < numElements; i++) {
        int response_time = taskList[i].num_tasks + (taskList[i].end_queue - taskList[i].begin_queue)+1;
        printf("%c\t%d\t%d\t%d\t\t%d\t\t%d\n", taskList[i].taskID+1, taskList[i].orginal_time, taskList[i].num_tasks, taskList[i].final_time+1, response_time, taskList[i].wait_time);
    }

    printf("\nservice wait\ntime\ttime\n");
    printf("-----------------------------------------------------------\n");
    int j = 0;
    for (int i = 0; i < numElements; i++) {
        printf("%d\t%d\n", taskList[i].num_tasks, taskList[i].wait_time);
    }

}





















