CC = gcc
CFLAGS= -Wall -g

TARGET = mzlib

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c ./mz/linkedlist.c ./mz/arraylist.c

clean:
	$(RM) $(TARGET)
