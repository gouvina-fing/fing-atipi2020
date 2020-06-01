CC = gcc
CFLAGS = -g -Wall

all: main

main: main.cpp
	$(CC) $(CFLAGS) -o main.x main.cpp src/denoiser.cpp

clean: 
	$(RM) *.o *.x