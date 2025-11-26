#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
//initalize buffer & size & count
int buffer[5];
int count = 0;
//declare semaphores 
sem_t empty;
sem_t full;
sem_t lock;

void* producer() {
    while (1) {
        int x = rand() % 100;
        //wait for empty slots
        sem_wait(&empty);
        //mutual exclusion
        sem_wait(&lock);
        //add item to a buffer
        buffer[count] = x;
        printf("Produced %d (count=%d)\n", x , count);
        count++;
        //unlock mutual exclusion
        sem_post(&lock);
        //signal that buffer is full
        sem_post(&full);
        usleep(200000); 
    }
}

void* consumer() {
    while (1) {
        int y;
        //wait for full slot 
        sem_wait(&full);
        //mutual exlusion
        sem_wait(&lock);
        y = buffer[count - 1];
        count--;
        printf("Consumed %d (count=%d)\n", y, count);
        sem_post(&lock);
        sem_post(&empty); 
        usleep(200000); //slow donw consumer
    }
}

int main() {
    srand(time(NULL));
    
    //initialize semaphores
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    sem_init(&lock, 0, 1);
    
    pthread_t th[2];
    pthread_create(&th[0], NULL, &producer, NULL);
    pthread_create(&th[1], NULL, &consumer, NULL);
    //threads wait for each other
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    return 0;
}
