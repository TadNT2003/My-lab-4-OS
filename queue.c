#include <stdlib.h>
#include "struct.h"
#include "queue.h"

// Initialize the queue
void initialize_queue(struct queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

// Check if a queue if empty
int empty(struct queue* q) {
    return (q->head == NULL);
}

// Put the process at the tail of the queue
void enqueue(struct pcb* process, struct queue* current_queue) {  
    if (current_queue->tail != NULL){
        current_queue->tail->next_in_queue = process;
        current_queue->tail = process;
    }
    else {
        current_queue->head = process;
        current_queue->tail = current_queue->head;
    }
}

// Take the process at the head of the queue
struct pcb* dequeue(struct queue* current_queue) {
    if (current_queue->head != NULL){
        struct pcb* temp = current_queue->head;
        if (current_queue->head == current_queue->tail) {
            current_queue->head = current_queue->tail = NULL;
        }
        else current_queue->head = current_queue->head->next_in_queue;
        return temp;
    }
    else return NULL;
}