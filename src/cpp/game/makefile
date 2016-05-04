CXX = clang++
CXXFLAGS = -g -std=c++11 -O2 -Wall

all: test

test: test.o game.o
	$(CXX) $(CXXFLAGS) -o test test.o game.o

test.o: test.cpp game.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp game.cpp

game.o: game.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

clean:
	$(RM) test *.o *~
