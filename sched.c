#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"

#define TIME_UNIT 100 // in milisecond

static int quantum_time;

static int load_done = 0;

static struct queue in_queue;
static struct queue ready_queue;

// Emulimate the cpu: take the pro and pre-empt if needed
void * cpu(void * arg) {
    int time_stamp = 0; // Time of the cpu

    while (!load_done || !empty(&ready_queue)) {
        struct pcb* pro = dequeue(&ready_queue);
        if (pro != NULL) {
            // Execute the process
            int start = time_stamp;
            int id = pro->pro_id;
            int exe_time;

            // Calculate the execution time of the process
            if (pro->burst_time <= quantum_time) {
                exe_time = pro->burst_time;
                pro->burst_time = 0;
            }
            else {
                exe_time = quantum_time;
                pro->burst_time -= quantum_time;
            }

            // Update time_stamp
            time_stamp += exe_time;
            usleep(exe_time * TIME_UNIT);

            // If a process burst time is 0 then free it, otherwise put it back to ready_queue
            if (pro->burst_time == 0) {
                free(pro);
            }
            else {
                enqueue(pro, &ready_queue);
            }

            /* Track runtime status */
			printf("%2d-%2d: Execute %d\n", start, time_stamp, id);
        }
        else {  // If there is no pro in ready_queue
            time_stamp++;
            usleep(TIME_UNIT);
        }
    }    

}

// Emulimate the loader: load pro to cpu
void * loader(void * arg) {
    int time_stamp = 0; // Time of the loader

    while (!empty(&in_queue)) {
        struct pcb* enter_pro = dequeue(&in_queue);
        // Make thread sleeps until arrival time
        int time_sleep = enter_pro->arrival_time - time_stamp;
        usleep(time_sleep * TIME_UNIT);
        time_stamp += time_sleep;
        enqueue(enter_pro, &ready_queue);
    }
    load_done = 1;
}

void load_task(){
    int num_of_pro;
    FILE *pro_file;
    pro_file = fopen("process.txt", "r");
    fscanf(pro_file, "%d %d", &quantum_time, &num_of_pro);
    for (int i = 0; i < num_of_pro; i ++) {
        struct pcb* new_process = (struct pcb*)malloc(sizeof(struct pcb));
        fscanf(pro_file, "%d %d", &new_process->arrival_time, &new_process->burst_time);
        new_process->pro_id = i;
        enqueue(new_process, &in_queue);
    }
}

int main() {
    pthread_t cpu_id;   // Thread for cpu, take its id
    pthread_t loader_id;    // Thread for loader, take its id

    // Initialize both queues
    initialize_queue(&in_queue);
    initialize_queue(&ready_queue);

    // Load task from file
    load_task();

    // Start cpu
    pthread_create(&cpu_id, NULL, cpu, NULL);
    // Start loader
    pthread_create(&loader_id, NULL, loader, NULL);

    // Wait for both threads to finish
    pthread_join(cpu_id, NULL);
    pthread_join(loader_id, NULL);

    pthread_exit(NULL);
}