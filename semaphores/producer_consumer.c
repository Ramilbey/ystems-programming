#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int buffer[5];
int count = 0;

sem_t empty;
sem_t full;
sem_t lock;
void* producer(void* arg) {
    while (1) {
        // Produce
        sem_wait(&empty);
        sem_wait(&lock);
        int x = rand() % 100;
        buffer[count] = x;
        printf("Produced %d (count=%d)\n", x , count);
        count++;
        sem_post(&lock);
        sem_post(&full);
        usleep(200000); 
    }
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);
        sem_wait(&lock);
        int y = -404;
        // Consume
        y = buffer[count - 1];
        count--;
        printf("Consumed %d (count=%d)\n", y, count);
        sem_post(&lock);
        sem_post(&empty); 
        usleep(200000); 
    }
}

int main() {
    srand(time(NULL));
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    sem_init(&lock, 0, 1);
    pthread_t th[2];
    pthread_create(&th[0], NULL, &producer, NULL);
    pthread_create(&th[1], NULL, &consumer, NULL);
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
    return 0;
}
