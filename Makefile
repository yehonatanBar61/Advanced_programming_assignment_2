.PHONY: all clean
CC=gcc
CFLAGS=-std=c11 -pthread -Wall -O2

all: randomGenerator primeCounter myPrimeCounter

randomGenerator: generator.c queue/lfq.o
	$(CC) $(CFLAGS) -o randomGenerator generator.c queue/lfq.o

primeCounter: primeCounter.c queue/lfq.o
	$(CC) $(CFLAGS) -o primeCounter primeCounter.c queue/lfq.o

myPrimeCounter: myPrimeCounter.c queue/lfq.o
	$(CC) $(CFLAGS) -o myPrimeCounter myPrimeCounter.c queue/lfq.o

queue/lfq.o: queue/lfq.c queue/cross-platform.h queue/lfq.h
	$(CC) $(CFLAGS) -Iqueue -c queue/lfq.c -o queue/lfq.o

clean:
	-rm -f randomGenerator primeCounter myPrimeCounter queue/lfq.o 2>/dev/null
