.PHONY: all clean
CC=gcc
CFLAGS=-std=c11 -pthread -Wall -O2

all: randomGenerator primesCounter primeCounterCP

randomGenerator: generator.c queue/lfq.o
	$(CC) $(CFLAGS) -o randomGenerator generator.c queue/lfq.o

primesCounter: primeCounter.c queue/lfq.o
	$(CC) $(CFLAGS) -o primesCounter primeCounter.c queue/lfq.o

primeCounterCP: primeCounterCP.c queue/lfq.o
	$(CC) $(CFLAGS) -o primeCounterCP primeCounterCP.c queue/lfq.o

queue/lfq.o: queue/lfq.c queue/cross-platform.h queue/lfq.h
	$(CC) $(CFLAGS) -Iqueue -c queue/lfq.c -o queue/lfq.o

clean:
	-rm -f randomGenerator primesCounter primeCounterCP queue/lfq.o 2>/dev/null
