# Makefile for compiling main.c with pthreads

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
TARGET = pth_linked_list.o
SRC = main.c linked_list.c linked_list.h

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) $(threads)

gen-csv: $(TARGET)
	./$(TARGET) -gen-csv $(samples)
