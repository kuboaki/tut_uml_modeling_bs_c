CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra
TARGET  = sample
SRCS    = main.c sample.c sample_test.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o:        main.c sample.h sample_test.h
sample.o:      sample.c sample.h
sample_test.o: sample_test.c sample.h sample_test.h

clean:
	$(RM) $(OBJS) $(TARGET)
