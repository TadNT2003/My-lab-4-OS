#ifndef QUEUE_H
#define QUEUE_H

#include "struct.h"

// Initialize a queue
void initialize_queue(struct queue* q);

// Check if a queue if empty
int empty(struct queue* q);

// Put the process at the tail of the queue
void enqueue(struct pcb* process, struct queue* current_queue);

// Take the process at the head of the queue
struct pcb* dequeue(struct queue* current_queue);

#endif