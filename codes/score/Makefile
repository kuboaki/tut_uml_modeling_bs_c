# Makefile for Bowling Score System

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
TARGET = bowling
OBJS = main.o bowling.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c bowling.h
	$(CC) $(CFLAGS) -c main.c

bowling.o: bowling.c bowling.h
	$(CC) $(CFLAGS) -c bowling.c

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)
