#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "fifo.h"

void* most(void* ptr);

node_t* Aqueue = NULL;
node_t* Bqueue = NULL;

int debug = 0;

pthread_mutex_t bridgeMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
        int carCount = 0;

        if(argc == 2) {
                carCount = atoi(argv[1]);
        }
        else if(argc == 3) {
                carCount = atoi(argv[1]);
                if(strcmp(argv[2], "-debug") == 0) {
                        debug = 1;
                }
                else {
                        fprintf(stderr, "Zly format wejscia - Poprawny: %s <N> (-debug)\n", argv[0]);
                        exit(EXIT_FAILURE);
                }
        }
        else {
                fprintf(stderr, "Zly format wejscia - Poprawny: %s <N> (-debug)\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        pthread_t cars[carCount];
        int carNumbers[carCount];

        for(int i = 0; i < carCount; i++) {
                carNumbers[i] = i;
        }

        for(int i = 0; i < carCount; i++) {
                if(i <= ((carCount-1)/2)) {
                        enqueue(&Aqueue, carNumbers[i]);
                }
                else {
                        enqueue(&Bqueue, carNumbers[i]);
                }
        }


        int iret1;
        for(int i = 0; i < carCount; i++) {
                iret1 = pthread_create( &cars[i], NULL, most, &carNumbers[i]);
                if(iret1) {
                        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
                        exit(EXIT_FAILURE);
                }
        }
        for (int i=0; i<carCount; i++) {
                pthread_join(cars[i], NULL);
        }

}

void* most(void* ptr) {
        const int carNumber = *(int*) ptr;
        srand(time(NULL)+carNumber);
        int randomTime = 0;


        while(1) {
                pthread_mutex_lock(&bridgeMutex);

                //Jezeli w kolejce A jest wiecej samochodow niz w kolejce B
                if(queue_length(Aqueue) >= queue_length(Bqueue)) {
                        if(queue_tail(Aqueue) == carNumber) {
                                randomTime = rand() % 1000000 + 200000;
                                usleep(randomTime);

                                if(debug == 1) {
                                        printf("-----------------------------------------\n");
                                        queue_print(Aqueue);
                                        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n",
                                        queue_length(Aqueue),
                                        queue_tail(Aqueue),
                                        carNumber,
                                        queue_tail(Bqueue),
                                        queue_length(Bqueue));
                                        queue_print(Bqueue);

                                }
                                else {
                                        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n",
                                        queue_length(Aqueue),
                                        queue_tail(Aqueue),
                                        carNumber,
                                        queue_tail(Bqueue),
                                        queue_length(Bqueue));
                                }

                                dequeue(&Aqueue);

                                enqueue(&Bqueue, carNumber);
                        }
                }
                //Jezeli w kolejce B jest wiecej samochodow niz w kolejce A
                else {
                        if(queue_tail(Bqueue) == carNumber) {
                                randomTime = rand() % 1000000 + 200000;
                                usleep(randomTime);

                                if(debug == 1) {
                                        printf("-----------------------------------------\n");
                                        queue_print(Aqueue);
                                        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n",
                                        queue_length(Aqueue),
                                        queue_tail(Aqueue),
                                        carNumber,
                                        queue_tail(Bqueue),
                                        queue_length(Bqueue));
                                        queue_print(Bqueue);

                                }
                                else {
                                        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n",
                                        queue_length(Aqueue),
                                        queue_tail(Aqueue),
                                        carNumber,
                                        queue_tail(Bqueue),
                                        queue_length(Bqueue));
                                }

                                dequeue(&Bqueue);

                                enqueue(&Aqueue, carNumber);
                        }
                }
                pthread_mutex_unlock(&bridgeMutex);
        }
        return NULL;
}
