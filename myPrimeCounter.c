#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue/lfq.h"
#include <unistd.h>
 
#define C_NUM_THREADS 8
#define P_NUM_THREADS 1
#define BlockSize 2000

typedef struct {
    int tid;       // thread id
    int counter;
    int Array[BlockSize]; 
} arg_set;


int total_counter; // using method 2 from the book
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool end_of_sdtin = false;


long long multiply_mod(long long a, long long b, long long mod) {
    long long result = 0;
    a = a % mod;
    while (b > 0) {
        if (b & 1)
            result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}

long long modular_exponentiation(long long base, long long exponent, long long mod) {
    if (exponent == 0)
        return 1;
    long long half = modular_exponentiation(base, exponent / 2, mod);
    long long half_squared = (half * half) % mod;
    
    if (exponent % 2 == 0)
        return half_squared;
    else
        return (half_squared * base) % mod;
}

int MillerRabin_isPrime(long long p, int iter) {
    if (p < 2 || (p != 2 && p % 2 == 0)) 
        return 0;
    long long s = p - 1;
    while (s % 2 == 0) {
        s /= 2;
    }
    for (int i = 0; i < iter; i++) {
        long long a = rand() % (p - 1) + 1;
        long long temp = s;
        long long mod = modular_exponentiation(a, temp, p);

        while (temp != p - 1 && mod != 1 && mod != p - 1) {
            mod = multiply_mod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
            return 0;
    }
    return 1;
}


bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}


void *consumer(void *arg) {
    arg_set *set = arg;

    while(1){
        pthread_mutex_lock(&lock);
        int i;
        int counter = 0;
        for(i = 0; i < BlockSize; i++){
            if(scanf("%d", &set->Array[i]) == EOF){
                end_of_sdtin = true; 
                break;
            }
            counter++;
        }
        pthread_mutex_unlock(&lock);
        
        if(counter == 0){
            return NULL;
        }

        for(int j = 0; j < counter; j++){
            if(MillerRabin_isPrime(set->Array[j], 4)){
                set->counter++;
                //printf("%d is prime (%d)\n", set->Array[j], j);
            }
        }
        //printf("total primes for this thread is: %d\n", set->counter);
    }
    
    return NULL;
    
}

int main() {
    pthread_t consumers[C_NUM_THREADS];
    arg_set consumer_set[C_NUM_THREADS];
    int total_number_of_primes = 0;

    // Create consumer threads
    for (int i = 0; i < C_NUM_THREADS; ++i) {
        consumer_set[i].tid = i;
        consumer_set[i].counter = 0;
        if (pthread_create(&consumers[i], NULL, consumer,(void*) &consumer_set[i]) != 0) {
            fprintf(stderr, "Failed to create consumer thread %d\n", i);
            return EXIT_FAILURE;
        }
    }
    
    for (int i = 0; i < C_NUM_THREADS; i++) {
        pthread_join(consumers[i], NULL);
        //printf("%d\n", consumer_set[i].counter);
        total_number_of_primes  += consumer_set[i].counter;
    }

    printf("total primes: %d\n", total_number_of_primes);

    return EXIT_SUCCESS;
}