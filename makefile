CC = gcc
CFLAGS = -c -Wall
all: test_sample test_queue

test_sample: test_sample.o lib-ult.o
	$(CC) test_sample.o lib-ult.o -lm -lpthread -o test_sample

test_queue: test_queue.o
	$(CC) test_queue.o -o test_queue

test_queue.o: test_queue.c
	$(CC) $(CFLAGS) test_queue.c

lib-ult.o:  lib-ult.c
	$(CC) $(CFLAGS) lib-ult.c

clean:
	rm *.o test_queue test_sample
