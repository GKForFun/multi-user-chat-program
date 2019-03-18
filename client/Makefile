CC = g++
CFLAGS = -std=c++11

all: main.o client.o
	$(CC) $(CFLAGS) main.o client.o -lpthread -o client

main.o: main.cpp
	$(CC) $(CFLAGS) -g -c main.cpp -lpthread

client.o: client.cpp client.h stdafx.h
	$(CC) $(CFLAGS) -g -c client.cpp -lpthread
