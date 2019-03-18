CC = g++
CFLAGS = -std=c++11

all: main.o server.o
	$(CC) $(CFLAGS) main.o server.o -lpthread -o server

main.o: main.cpp
	$(CC) $(CFLAGS) -g -c main.cpp -lpthread

server.o: server.cpp server.h
	$(CC) $(CFLAGS) -g -c server.cpp -lpthread
