#include <stdio.h>
#include <stdlib.h>
#include <fifo.h>

void enqueue(node_t** head, int dane) {
        node_t* node = malloc(sizeof(node_t));

        node->car = dane;
        node->next = NULL;

        node->next = *head;
        *head = node;
}

void dequeue(node_t** head) {
        if(*head == NULL) {
                return;
        }

        if((*head)->next == NULL) {
                free(*head);
                *head = NULL;
                return;
        }

        node_t** current = head;

        while((*current)->next->next != NULL) {
                current = &(*current)->next;
        }

        free((*current)->next);
        (*current)->next = NULL;
}


int queue_tail(node_t* head) {
        if(head == NULL) {
                return -1;
        }

        while(head->next != NULL) {
                head = head->next;
        }
        return head->car;
}

int queue_length(node_t* head) {
        if(head == NULL) {
                return 0;
        }

        int len = 0;
        while(head != NULL) {
                head = head->next;
                len++;
        }
        return len;
}

void queue_print(node_t* head) {
        while(head != NULL) {
                printf("%d ", head->car);
                head = head->next;
        }
        printf("\n");
}
