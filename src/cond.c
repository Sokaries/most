#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fifo.h>

int main(void) {
        node_t* queue1 = NULL;

        for(int i = 10; i < 20; i++) {
                enqueue(&queue1, i);
                queue_print(queue1);
        }

        for(int i = 0; i < 10; i++) {
                queue_print(queue1);
        }
}
