#ifndef STRUCT_H
#define STRUCT_H

struct pcb {
    int pro_id;
    int arrival_time;
    int burst_time;
    struct pcb* next_in_queue;
};


struct queue {
    struct pcb* head;
    struct pcb* tail;
};

#endif