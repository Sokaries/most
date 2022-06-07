#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "fifo.h"

#define Acity 0
#define Bcity 1


void bridge();
void* car(void* ptr);
void incCity(int city);
void decCity(int city);
void syncEnqueue(node_t** queue, int carNumber);
void syncDequeue(node_t** queue);

node_t* Aqueue = NULL;
node_t* Bqueue = NULL;
int cityCount[2] = {0, 0};

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bridgeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cityCountMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t terminalMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
        if(argc != 2) {
                printf("Zle podane parametry, format: '%s <N>'\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        int carCount = atoi(argv[1]);

        pthread_t cars[carCount];
        int carNumbers[carCount];

        for(int i = 0; i < carCount; i++) {
                carNumbers[i] = i;
        }

        int iret1;
        for(int i = 0; i < carCount; i++) {
                iret1 = pthread_create( &cars[i], NULL, car, &carNumbers[i]);
                if(iret1) {
                        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
                        exit(EXIT_FAILURE);
                }
        }
        for (int i=0; i<carCount; i++) {
                pthread_join(cars[i], NULL);
        }
}

void bridge(int carNumber, int* city, int* didCross) {
        pthread_mutex_lock(&queueMutex);
        int randomTime = 0;
        //Jezeli w kolejce A jest wiecej samochodow niz w kolejce B
        if(queue_length(Aqueue) >= queue_length(Bqueue)) {
                if(queue_tail(Aqueue) == carNumber) {

                        pthread_mutex_lock(&cityCountMutex);
                        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n",
                               cityCount[Acity],
                               queue_length(Aqueue),
                               carNumber,
                               queue_length(Bqueue),
                               cityCount[Bcity]);
                        pthread_mutex_unlock(&cityCountMutex);
                        pthread_mutex_unlock(&queueMutex);

                        randomTime = rand() % 1000000 + 200000;
                        usleep(randomTime);

                        syncDequeue(&Aqueue);

                        *city = Bcity;
                        *didCross = 1;

                        incCity(Bcity);

                        return;
                } else {
                        *didCross = 0;
                        pthread_mutex_unlock(&queueMutex);
                        return;
                }
        }
        //Jezeli w kolejce B jest wiecej samochodow niz w kolejce A
        else {
                if(queue_tail(Bqueue) == carNumber) {

                        pthread_mutex_lock(&cityCountMutex);
                        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n",
                               cityCount[Acity],
                               queue_length(Aqueue),
                               carNumber,
                               queue_length(Bqueue),
                               cityCount[Bcity]);
                        pthread_mutex_unlock(&cityCountMutex);
                        pthread_mutex_unlock(&queueMutex);

                        randomTime = rand() % 2000000 + 500000;
                        usleep(randomTime);

                        syncDequeue(&Bqueue);

                        *city = Acity;
                        *didCross = 1;

                        incCity(Acity);
                        return;
                } else {
                        *didCross = 0;
                        pthread_mutex_unlock(&queueMutex);
                        return;
                }
        }

}

void* car(void* ptr) {
        //Setup
        const int carNumber = *(int*) ptr;
        srand(time(NULL)+carNumber);

        //Wybor miasta startowego - polowa w A, polowa w B
        int city = carNumber%2 ? Acity : Bcity;
        int didCross = 1;

        //Dodaj do licznika
        incCity(city);

        int randomTime = 0;

        while(1) {
                if(didCross == 1) {
                        //Wylicz czas zanim wejdziesz do kolejki przejazdu
                        randomTime = rand() % 2000000 + 250000;
                        usleep(randomTime);

                        if(city == Acity) {
                                syncEnqueue(&Aqueue, carNumber);
                                decCity(city);
                        }
                        else {
                                syncEnqueue(&Bqueue, carNumber);
                                decCity(city);
                        }
                }

                pthread_mutex_lock(&bridgeMutex);
                bridge(carNumber, &city, &didCross);
                pthread_mutex_unlock(&bridgeMutex);
        }
        return NULL;
}

void syncEnqueue(node_t** queue, int carNumber) {
        pthread_mutex_lock(&queueMutex);
        enqueue(queue, carNumber);
        pthread_mutex_unlock(&queueMutex);
}

void syncDequeue(node_t** queue) {
        pthread_mutex_lock(&queueMutex);
        dequeue(queue);
        pthread_mutex_unlock(&queueMutex);
}

void incCity(int city) {
        pthread_mutex_lock(&cityCountMutex);
        cityCount[city]++;
        pthread_mutex_unlock(&cityCountMutex);
}

void decCity(int city) {
        pthread_mutex_lock(&cityCountMutex);
        cityCount[city]--;
        pthread_mutex_unlock(&cityCountMutex);
}
