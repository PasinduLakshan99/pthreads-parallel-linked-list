# Makefile for compiling main.c with pthreads

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
TARGET = pth_linked_list.o
SRC = main.c linked_list.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
#run target with default number of 8 threads and overide the defaukt number of threads when threads = <no.of.threads> is passed as argument
	./$(TARGET) $(threads)