#  Copyright 2017 Adrien Forkum
CC = g++
CFLAGS = -c -std=c++11 -g -Wall -Werror -pedantic
INCLUDE = -L /usr/local/lib
LFLAGS = -lboost_regex

all: main.o ps7b.o
	$(CC) -o ps7b main.o ps7b.o $(LFLAGS) $(INCLUDE)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

ps7b.o: ps7b.cpp
	$(CC) $(CFLAGS) ps7b.cpp

clean:
	rm *.exe *.o *.gch