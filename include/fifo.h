#ifndef FIFO_H
#define FIFO_H

typedef struct node {
        int car;
        struct node* next;
} node_t;

void enqueue(node_t** head, int dane);

void dequeue(node_t** head);

int queue_tail(node_t* head);

int queue_length(node_t* head);

void queue_print(node_t* head);

#endif
