#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
int total_words;

int main(int ac, char *av[]){

    pthread_t t1, t2;

    void *cout_words(void *);

    printf("number of args is: %d", ac);

    if(ac != 3){
        printf("usage: %s file1 file2\n" , av[0]);
        exit(1);
    }
    total_words = 0;

    pthread_create(&t1, NULL, cout_words, (void *)av[1]);
    pthread_create(&t2, NULL, cout_words, (void *)av[2]);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%5d: total words\n", total_words);
}


void *cout_words(void *f){
    char * file_name = (char *) f;
    FILE *fd;
    int c, prevc = '\0';

    if((fd = fopen(file_name, "r")) != NULL){
        while((c = getc(fd)) != EOF){
            if(!isalnum(c) && isalnum(prevc)){
                total_words++;
            }
            prevc = c;
        }
        fclose(fd);
    }else
        perror(file_name);
    return NULL;
}