#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

/*no mutex - it takes to time, we pass multiple argument for each thread*/

typedef struct {
    char* file_name;       // the file descriptor
    int counter;  // counts the number of words in the file
} arg_set;


int main(int ac, char *av[]){

    pthread_t t1, t2;

    void *cout_words(void *);

    printf("number of args is: %d", ac);

    if(ac != 3){
        printf("usage: %s file1 file2\n" , av[0]);
        exit(1);
    }

    arg_set s1, s2;
    s1.counter = 0;
    s2.counter = 0;
    s1.file_name = av[1];
    s2.file_name = av[2];

    pthread_create(&t1, NULL, cout_words, (void *)&s1);
    pthread_create(&t2, NULL, cout_words, (void *)&s2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%5d: total words\n", s1.counter + s2.counter);
}


void *cout_words(void *set){
    arg_set * args = set;
    FILE *fd;
    int c, prevc = '\0';

    if((fd = fopen(args->file_name, "r")) != NULL){
        while((c = getc(fd)) != EOF){
            if(!isalnum(c) && isalnum(prevc)){
                args->counter++;
            }
            prevc = c;
        }
        fclose(fd);
    }else
        perror(args->file_name);
    return NULL;
}