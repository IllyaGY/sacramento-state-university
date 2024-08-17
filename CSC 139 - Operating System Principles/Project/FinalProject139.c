// CSC139-02 Project for Producer-Consumer Problem
// Summer 2024 Session II 
// Team members: Mansoor Ali, Christian Ramirez, Illya Gordyy, Bilal Baloch
// The objective of the project is to complete a multi-threaded synchronization problem using 
// semaphores and mutexes in a partially completed project code.
// Websites referenced above the code that was used.
//Sample code for producer/consumer problem

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS 20

int buffer1[BUFFER_SIZE];
int buffer1_count = 0;
int buffer1_in = 0;
int buffer1_out = 0;
pthread_mutex_t buffer1_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t buffer1_full;
sem_t buffer1_empty;

int buffer2[BUFFER_SIZE];
int buffer2_count = 0;
int buffer2_in = 0;
int buffer2_out = 0;
pthread_mutex_t buffer2_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t buffer2_full;
sem_t buffer2_empty;

void produce(int* buffer, int* buffer_count, int* buffer_in, pthread_mutex_t* buffer_mutex, sem_t* buffer_full, sem_t* buffer_empty, int item) {
    sem_wait(buffer_empty);
    pthread_mutex_lock(buffer_mutex);
    
    buffer[*buffer_in] = item;
    *buffer_in = (*buffer_in + 1) % BUFFER_SIZE;
    (*buffer_count)++;
    
    pthread_mutex_unlock(buffer_mutex);
    sem_post(buffer_full);
}

int consume(int* buffer, int* buffer_count, int* buffer_out, pthread_mutex_t* buffer_mutex, sem_t* buffer_full, sem_t* buffer_empty) {
    sem_wait(buffer_full);
    pthread_mutex_lock(buffer_mutex);
    
    int item = buffer[*buffer_out];
    *buffer_out = (*buffer_out + 1) % BUFFER_SIZE;
    (*buffer_count)--;
    
    pthread_mutex_unlock(buffer_mutex);
    sem_post(buffer_empty);
    return item;
}

void* producer(void* arg) {
    int producer_id = *(int*)arg;
   // printf("The producer ID is %d \n",producer_id);
    for (int i = 0; i < NUM_ITEMS; i++) {
        // printf("The producer ID is %d \n",producer_id);
        int item = producer_id * 100 + i;
        produce(buffer1, &buffer1_count, &buffer1_in, &buffer1_mutex, &buffer1_full, &buffer1_empty, item);
        printf("Producer ID: %d, Produced: %d into Buffer 1\n", producer_id, item);
        usleep((rand() % 500) * 1000);
    }
    return NULL;
}

void* first_level_consumer_producer(void* arg) {
    int consumer_producer_id = *(int*)arg;
    //printf("first level consumer producerID: %d \n",consumer_producer_id);
    for (int i = 0; i < NUM_ITEMS; i++) {
        // printf("first level consumer producer ID: %d \n",consumer_producer_id);
        int item = consume(buffer1, &buffer1_count, &buffer1_out, &buffer1_mutex, &buffer1_full, &buffer1_empty);
        printf("Consumer ID: %d , Consumed: %d from Buffer 1\n", consumer_producer_id, item);
        int new_item = item * 10 + consumer_producer_id;
        produce(buffer2, &buffer2_count, &buffer2_in, &buffer2_mutex, &buffer2_full, &buffer2_empty, new_item);
        printf("Producer ID: %d , Produced: %d into Buffer 2\n", consumer_producer_id, new_item);
        usleep((rand() % 500) * 1000);
    }
    return NULL;
}

void* second_level_consumer(void* arg) {
    int consumer_id = *(int*)arg;
    //printf("Second level consumer ID: %d \n",consumer_id);
    for (int i = 0; i < NUM_ITEMS; i++) {
        // printf("Second level consumer ID: %d \n",consumer_id);
        int item = consume(buffer2, &buffer2_count, &buffer2_out, &buffer2_mutex, &buffer2_full, &buffer2_empty);
        printf("Consumer_ID: %d ,Consumed: %d from Buffer 2\n", consumer_id, item);
        usleep((rand() % 500) * 1000);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    //Website utilized: https://www.tutorialspoint.com/producer-consumer-problem-in-c
    
    // Declare threads for all types of producers and consumers
    pthread_t producerThread, firstLevelConsumerProducerThread, secondLevelConsumerThread;
    // Declare variables for 2 producers entity, 2 producer/consmuer entity and 2 consumers  
    //Website utilized: https://stackoverflow.com/questions/40699045/what-does-int-arg-do
    //Based on functions taking int consumer_id = *(int*)arg; from argument of function, I assume
    //we put the variable values into the threaded functions 4th parameter which serves as argument for the threaded
    //function. This is why I created the variables as integers with values indicating their id.
    int prod1=1,prod2=2,prodcon1=3,prodcon2=4,con1=5,con2=6;

    // Initialize semaphores
    sem_init(&buffer1_full, 0, 0);
    sem_init(&buffer1_empty, 0, BUFFER_SIZE);

    sem_init(&buffer2_full, 0, 0);
    sem_init(&buffer2_empty, 0, BUFFER_SIZE);


    // create threads
    //Utilized this website: https://man7.org/linux/man-pages/man3/pthread_create.3.html
    pthread_create(&producerThread, NULL, producer, &prod1);
    pthread_create(&producerThread, NULL, producer, &prod2);
    pthread_create(&firstLevelConsumerProducerThread, NULL, first_level_consumer_producer, &prodcon1);
    pthread_create(&firstLevelConsumerProducerThread, NULL, first_level_consumer_producer, &prodcon2);
    pthread_create(&secondLevelConsumerThread, NULL, second_level_consumer, &con1);
    pthread_create(&secondLevelConsumerThread, NULL, second_level_consumer, &con2);

    //Join threads
    //Utilized this website: https://man7.org/linux/man-pages/man3/pthread_join.3.html
    pthread_join(producerThread, NULL);
    pthread_join(firstLevelConsumerProducerThread, NULL);
    pthread_join(secondLevelConsumerThread, NULL);

    //destroy threads
    pthread_mutex_destroy(&buffer1_mutex);
    pthread_mutex_destroy(&buffer2_mutex);

    //--Not prof code: destroy semaphore not needed?
    // sem_destroy(&buffer1_full);
    // sem_destroy(&buffer1_empty);

    // sem_destroy(&buffer2_full);
    // sem_destroy(&buffer2_empty);

    printf("All producers and consumers have finished.\n");

    return 0;
}