#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM  5
int counter = 0;

int main(){
    pthread_t t1;
    void *print_count(void *);

    pthread_create(&t1, NULL, print_count, NULL);
    int i;
    for(i = 0; i < NUM; i++){
        counter++;
        sleep(1);
    }

    pthread_join(t1, NULL);
    return 1;
}

void *print_count(void *m){
    for(int i = 0; i < NUM; i++){
        printf("cout = %d\n", counter);
        sleep(1);
    }
    return NULL;
}


