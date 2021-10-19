// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// asg2.c

/*
 * flag = 0 | First in, first out (FIFO)
 *      First in, first out (FIFO), also known as first come, first served (FCFS), is the simplest scheduling
 *      algorithm. FIFO simply queues processes in the order that they arrive in the ready queue. In this, the
 *      process that comes first will be executed first and next process starts only after the previous gets
 *      fully executed.
 *
 * flag = 1 | Shortest job first (SJF)
 *      Shortest job first (SJF) or shortest job next, is a scheduling policy that selects the waiting process
 *      with the smallest execution time to execute next. It is a very greedy algorithm. Shortest Job first has
 *      the advantage of having a minimum average waiting time among all scheduling algorithms.
 *
 * flag = 2 | Round robin (RR)
 *      A round-robin is a CPU scheduling algorithm that shares equal portions of resources in circular orders
 *      to each process and handles all processes without prioritization. In the round-robin, each process gets
 *      a fixed time interval of the slice to utilize the resources or execute its task called time quantum or
 *      time slice. Some of the round-robin processes are pre-empted if it executed in a given time slot, while
 *      the rest of the processes go back to the ready queue and wait to run in a circular order with the scheduled
 *      time slot until they complete their task. It removes the starvation for each process to achieve CPU
 *      scheduling by proper partitioning of the CPU.
 *
 *
 *  The last thing worth mentioning: the nested if statements will do all of the calculation and then will print
 *  after everything is completed. All of the three policies print the same.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_TASKS 26
#define FIFO 0
#define SJF 1
#define RR 2
#define TRUE 1
#define FALSE 0

// Structure provided
struct task{
    int
            task_id,       /* alphabetic tid can be obtained as 'A'+(numTask++) */
    arrival_time,
            service_time,
            remaining_time,
            completion_time,
            response_time,
            wait_time,
            printed; // printed will be useful for determining if the specific task has printed yet
    struct task *next;
};

int main(int argc, char **argv){
    // Below are global variables that are either used throughout all policies or outside of their functions.
    // flag is the policy that we will be selecting from the CLA. 0 - FIFO | 1 - SJF | 2 - RR
    int flag = 0, numTask = 0, clock = 0, numTasks = 0;

    struct task *tasks[MAX_TASKS]; // This will be our linked list of tasks that are provided through redirection
    struct task *rover, *temp;     // rover will help us navigate through the linked list. temp is for copying data

    int w = 0; // w - temp loop var
    while (w < MAX_TASKS){ tasks[w] = malloc(sizeof(struct task)); w++;} // Create our linked list elements

    // Select specific policy based on CLA
    if(strcmp(argv[1], "-fifo") == 0)   { flag = FIFO; }
    else if(strcmp(argv[1],"-sjf") == 0){ flag = SJF; }
    else if(strcmp(argv[1],"-rr") == 0) { flag = RR; }
    else                                { printf("Error: Invalid CLA...\n"); exit(1); }

    // We need to check two things while scanning for our input values.
    //      1. We have not reached the end of file (!feof(stdin)) while scanning
    //      2. We have no read in more than 26 pairs of inputs
    while(fscanf(stdin,"%d %d", &tasks[numTask]->arrival_time, &tasks[numTask]->service_time) && !feof(stdin) && numTask < 25) {
        tasks[numTask]->task_id = 'A' + (numTask);
        tasks[numTask]->remaining_time = tasks[numTask]->service_time;
        tasks[numTask]->next = tasks[numTask+1]; // Pointer to next element
        tasks[numTask]->printed = 0; // This will be used later. We'll just set them all to 0 for now.
        numTask++;
    }
    // Once we've finished filling our linked list we need to do some cleaning up.
    // The last element is pointing to an element that we don't have so we need to change that to NULL
    tasks[numTask]->task_id = 'A' + (numTask);
    tasks[numTask]->remaining_time = tasks[numTask]->service_time;
    tasks[numTask]->printed = 0;
    tasks[numTask]->next=NULL;

    switch (flag) {
        case FIFO:
            printf("FIFO scheduling results\n\ntime  cpu  ready queue (tid/rst)\n");
            printf("----------------------------------\n");
            clock = 0;
            for(int i = 0; i <= numTask; i++){
                while(tasks[i]->arrival_time > clock){ // Check if we have a task within the current clock time, if not
                    printf(" %2d        --\n", clock); // keep counting until we do
                    clock++;

                }

                while(tasks[i]->remaining_time > 0) { // tasks[i] going to run for the duration of it's remaining time
                    printf(" %2d   %c%d  ", clock, tasks[i]->task_id, tasks[i]->remaining_time);

                    rover = tasks[i]->next; // We need to check if there is something in the queue coming up.
                    int queue = 0;     // # of items in the ready list
                    if(rover == NULL){      // If there is nothing on CPU then just print queue dashes, otherwise print the list
                        printf(" --\n");
                    } else{
                        while(rover != NULL){ // Determine if tasks are also wanting to run at this time
                            if(rover->arrival_time <= clock){
                                if(queue == 0){ // Only one task in the queue
                                    printf(" %c%d", rover->task_id, rover->remaining_time);
                                } else {             // For multiple tasks in the queue we need to print a comma
                                    printf(", %c%d", rover->task_id, rover->remaining_time);
                                }
                                rover->wait_time++; // Since it isn't on the CPU yet we need to record it's wait time
                                queue++;
                            } else {
                                if(queue == 0) {
                                    printf(" --"); // No elements in the queue then stop looping
                                }
                                break;
                            }
                            rover = rover->next;
                        }
                        printf("\n"); // End of each second
                    }
                    tasks[i]->remaining_time--;
                    clock++;
                }
                tasks[i]->completion_time = clock; // Determine how long it took to run
                tasks[i]->response_time = tasks[i]->completion_time - tasks[i]->arrival_time - 1;
            }
            break;
        case SJF:
            // SJF is very similar to FIFO, just a few small changes.

            printf("SJF(preemptive) scheduling results\n\ntime  cpu  ready queue (tid/rst)");
            printf("\n----------------------------------\n");
            int ptr;
            clock = 0;
            numTasks = numTask+1;

            while(numTasks > 0){
                int running = 0;
                for(int i = 0; i < numTask; i++){ // Waiting for a task to arrive
                    if(tasks[i]->remaining_time > 0 && tasks[i]->arrival_time <= clock){
                        running++;
                    }
                }
                if(running == 0){
                    printf(" %2d        --\n", clock); clock++;
                } else {
                    int smallestTime = 0;
                    for(int j = 0; j <= numTask; j++) { // We need to find the SMALLEST runtime and let that one go first
                        if(smallestTime == 0 && tasks[j]->remaining_time > smallestTime){
                            smallestTime = tasks[j]->remaining_time + 1;
                        }
                        // if the task has arrived and we've determined that it's the smallest then set ptr to it's value
                        if(tasks[j]->arrival_time <= clock && tasks[j]->remaining_time < smallestTime && tasks[j]->remaining_time > 0){
                            smallestTime = tasks[j]->remaining_time;
                            ptr = j;
                        }
                    }
                    printf(" %2d   %c%d  ", clock, tasks[ptr]->task_id,tasks[ptr]->remaining_time);
                    int queue = 0;
                    for(int j = 0; j <= numTask; j++){
                        if(tasks[j]->arrival_time <= clock && tasks[j]->remaining_time > 0 && tasks[j]->task_id != 'A'+(ptr)){
                            if(queue == 0){ // The only item in the queue
                                printf(" %c%d", tasks[j]->task_id,tasks[j]->remaining_time);
                                queue++;
                                tasks[j]->wait_time++;
                            } else { // More than one item in the queue
                                printf(", %c%d", tasks[j]->task_id,tasks[j]->remaining_time);
                                tasks[j]->wait_time++; // Determine how long it has to wait
                            }
                        } if (queue == 0 && j == numTask) printf(" --"); // Nothing queued
                    }
                    printf("\n");
                    tasks[ptr]->remaining_time--;
                    clock++;

                    if(tasks[ptr]->remaining_time == 0) { // Finished that task so record it's details
                        tasks[ptr]->completion_time = clock;
                        tasks[ptr]->response_time = tasks[ptr]->completion_time - tasks[ptr]->arrival_time - 1;
                        numTasks--;
                    }
                }
            }
            break;
        case RR:
            printf("RR scheduling results (time slice is 1)\n\ntime  cpu  ready queue (tid/rst)");
            printf("\n----------------------------------\n");
            clock = 0;
            numTasks = numTask+1;
            rover = tasks[numTask];

            // Copied from SJF: Waiting for a task to run
            while(numTasks > 0) {
                int running = FALSE;
                for(int i = 0; i <= numTask; i++){
                    if(tasks[i]->remaining_time > 0 && tasks[i]->arrival_time <= clock){
                        running = TRUE;
                    }
                }
                if(running == FALSE){ // No task to run so just print empty
                    printf(" %2d        --\n", clock); clock++;
                } else { // Finally have a task to run
                    int found = FALSE;
                    while(found == FALSE) { // Find the next task that will run
                        rover = rover->next;
                        if(rover == NULL) rover = tasks[0]; // If we're at the end of the tasks then go back to the start
                        if(rover->remaining_time > 0 && rover->arrival_time <= clock) found = TRUE; // Within bounds
                    }
                    printf(" %2d   %c%d   ", clock, rover->task_id, rover->remaining_time); // Next task to run
                    rover->remaining_time--;

                    if(rover->remaining_time == 0) { // Task has ended so record it's data for the table
                        numTasks--;
                        rover->completion_time = clock + 1;
                        rover->response_time = rover->completion_time-rover->arrival_time - 1;
                    }

                    // Build our queue just like we did in FIFO and SJF
                    int queue = 0;
                    temp = rover->next;
                    if(temp == NULL) temp = tasks[0]; // Ensure we're not at the last task
                    while(temp != rover) { // We don't want to over check
                        if(temp->remaining_time > 0 && temp->arrival_time <= clock) {
                            if(queue == 0) { // 1 element in queue
                                printf("%c%d", temp->task_id,temp->remaining_time);
                            } else { // more than one element
                                printf(", %c%d", temp->task_id,temp->remaining_time);
                            }
                            temp->wait_time++;
                            queue++;
                        }
                        temp=temp->next;
                        if (temp == NULL) temp = tasks[0]; // Go back to the first task
                    }
                    if(queue == 0) { // Nothing in the queue
                        printf("--\n");
                    } else { // Something was printed
                        printf("\n");
                    }
                    clock++;
                }
            }
            break;
        default:
            printf("Error: Exiting...\n");
            exit(1);
    }

    /* ----------------------------------------------------------
     *  Below will print the tables as shown in the example file.
     *  I'm sorry if it doesn't exactly match your test cases :/
     *  I tried to make it as close as I could.
     * ----------------------------------------------------------
     */

    printf("\n     arrival service completion response wait\ntid   time    time      time      time   time");
    printf("\n---------------------------------------------\n");

    rover = tasks[0]; // Start at the first task
    while(rover != NULL){
        printf(" %c     %2d      %2d       %2d        %2d     %2d\n",
               rover->task_id,rover->arrival_time,rover->service_time, rover->completion_time,rover->response_time+1,rover->wait_time);
        rover = rover->next;
    }
    printf("\nservice wait\n time   time\n--------------\n");
    int foo = 0;
    for(int i = 0; i < numTask; i++) {
        if(tasks[i]->service_time >= foo) foo = tasks[i]->service_time; // Find max service time
    }
    while(numTask >= 0){
        int i, max = foo;
        rover = tasks[0];
        while(rover != NULL){
            if(rover->service_time <= max && rover->printed != 1){ // After we print the max we need to set the next max
                max = rover->service_time;
                i = (int)(rover->task_id - 'A');
            }
            rover = rover->next;
        }
        printf("  %2d     %2d\n", tasks[i]->service_time, tasks[i]->wait_time);
        tasks[i]->printed = 1; // Ensure we don't print the same task again
        numTask--;
    }

    // ----------------------------------------------------------

    // Return allocated memory before we exit. Verify that all memory has released with
    // valgrind --leak-check=yes ./a.out -fifo < in1
    for(int i = 0; i < MAX_TASKS; i++) free(tasks[i]);

    return 0;
}