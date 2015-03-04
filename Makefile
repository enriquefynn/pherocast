CC=c++
CCFLAGS=-std=c++0x -march=native -Wall -g -O3

all: parser.o node.o graph.o
	$(CC) $(CCFLAGS) node.o graph.o parser.o -o main
parser.o: parser.cpp
	$(CC) $(CCFLAGS) -c parser.cpp
taxi_parser.o: taxi_parser.cpp
	$(CC) $(CCFLAGS) -c taxi_parser.cpp
node.o: node.cpp
	$(CC) $(CCFLAGS) -c node.cpp
graph.o: graph.cpp graph.hpp
	$(CC) $(CCFLAGS) -c graph.cpp
taxi: taxi_parser.o node.o graph.o
	$(CC) $(CCFLAGS) node.o graph.o taxi_parser.o -o taxi_parser
clean:
	rm -Rf *.o main
