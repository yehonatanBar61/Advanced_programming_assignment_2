#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue/lfq.h"
#include <unistd.h>
 
#define C_NUM_THREADS 4
#define P_NUM_THREADS 1

typedef struct {
    int tid;       // thread id
    int counter; 
} arg_set;


int total_counter; // using method 2 from the book
struct lfq_ctx queue;
int total_dequeued_number;
int total_enqueued_number;
int producer_finished = 0;

pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;


void *producer(void *arg) {
    arg_set *set = arg;
    int tid = set->tid;
    int num;
    while (scanf("%d", &num) != EOF) {   
        int *data = malloc(sizeof(int));
        *data = num;
        set->counter++;
        lfq_enqueue(&queue, data);
        printf("Producer %d enqueued %d\n", tid, *data);
    }
    producer_finished = 1;
    return NULL;
}

void *consumer(void *arg) {
    arg_set *set = arg;
    int tid = set->tid;
    
    int *data;
    while(1) {
        data = (int *)lfq_dequeue_tid(&queue, tid);
        if (data) {
            set->counter++;
            printf("Consumer %d dequeued %d\n", tid, *data);
            free(data);
        } else {
            printf("Consumer %d found queue empty\n", tid);
            if(producer_finished){
                break;
            }
        }
        
    }
    return NULL;
}

int main() {
    pthread_t producers[P_NUM_THREADS], consumers[C_NUM_THREADS];
    arg_set producer_set[P_NUM_THREADS], consumer_set[C_NUM_THREADS];

    // Initialize the LFQ
    if (lfq_init(&queue, C_NUM_THREADS) != 0) {
        fprintf(stderr, "Failed to initialize LFQ\n");
        return EXIT_FAILURE;
    }

    
    // Create producer threads
    for (int i = 0; i < P_NUM_THREADS; ++i) {  
        producer_set[i].tid = i;
        producer_set[i].counter = 0;
        if (pthread_create(&producers[i], NULL, producer, &producer_set[i]) != 0) {
            fprintf(stderr, "Failed to create producer thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Create consumer threads
    for (int i = 0; i < C_NUM_THREADS; ++i) {
        consumer_set[i].tid = i;
        consumer_set[i].counter = 0;
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_set[i]) != 0) {
            fprintf(stderr, "Failed to create consumer thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < P_NUM_THREADS; i++) {
        total_enqueued_number += producer_set[i].counter;
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < C_NUM_THREADS; i++) {
        total_dequeued_number += consumer_set[i].counter;
        pthread_join(consumers[i], NULL);
    }

    printf("out: %d in: %d\n", total_dequeued_number, total_enqueued_number);

    // Clean up the LFQ
    if (lfq_clean(&queue) != 0) {
        fprintf(stderr, "Failed to clean up LFQ\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
