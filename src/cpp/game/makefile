CC = g++
CFLAGS = -O2 -Wall

all: test

test: test.o game.o
	$(CC) $(CFLAGS) -o test test.o game.o

test.o: test.cpp game.cpp
	$(CC) $(CFLAGS) -c test.cpp game.cpp

game.o: game.cpp
	$(CC) $(CFLAGS) -c game.cpp

clean:
	$(RM) test *.o *~
