#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue/lfq.h"

#define C_NUM_THREADS 4
#define P_NUM_THREADS 1
int total_counter; // using method 2 from the book
struct lfq_ctx queue;
int total_dequeued_number;
int total_enqueued_number;

pthread_mutex_t dequeued_lock = PTHREAD_MUTEX_INITIALIZER; // the lockes limites the program to be fast
pthread_mutex_t enqueued_lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    int tid = *(int *)arg;


    int num;
    while (scanf("%d", &num) != EOF) {   
        int *data = malloc(sizeof(int));
        *data = num;
        lfq_enqueue(&queue, data);
        pthread_mutex_lock(&enqueued_lock);
        total_enqueued_number++;
        pthread_mutex_unlock(&enqueued_lock);
        printf("Producer %d enqueued %d\n", tid, *data);
    }
    return NULL;
}

void *consumer(void *arg) {
    int tid = *(int *)arg;
    int *data;

    while(1) {
        data = (int *)lfq_dequeue_tid(&queue, tid);
        if (data) {
            pthread_mutex_lock(&dequeued_lock);
            total_dequeued_number++;
            pthread_mutex_unlock(&dequeued_lock);
            printf("Consumer %d dequeued %d\n", tid, *data);
            free(data);
        } else {
            printf("Consumer %d found queue empty\n", tid);
            break;
        }
    }
    return NULL;
}

int main() {
    pthread_t producers[P_NUM_THREADS], consumers[C_NUM_THREADS];
    int producer_ids[P_NUM_THREADS];
    int consumer_ids[C_NUM_THREADS];

    // Initialize the LFQ
    if (lfq_init(&queue, C_NUM_THREADS) != 0) {
        fprintf(stderr, "Failed to initialize LFQ\n");
        return EXIT_FAILURE;
    }

    // Create producer threads
    for (int i = 0; i < P_NUM_THREADS; ++i) {  
        producer_ids[i] = i;
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
            fprintf(stderr, "Failed to create producer thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Create consumer threads
    for (int i = 0; i < C_NUM_THREADS; ++i) {
        consumer_ids[i] = i;
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]) != 0) {
            fprintf(stderr, "Failed to create consumer thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < P_NUM_THREADS; ++i) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < C_NUM_THREADS; ++i) {
        pthread_join(consumers[i], NULL);
    }

    printf("%d %d\n", total_dequeued_number, total_enqueued_number);

    // Clean up the LFQ
    if (lfq_clean(&queue) != 0) {
        fprintf(stderr, "Failed to clean up LFQ\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
