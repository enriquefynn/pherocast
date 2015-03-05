CC=c++
CCFLAGS=-std=c++0x -march=native -Wall -g -O3

all: parser.o node.o graph.o config.o jsoncpp.o
	$(CC) $(CCFLAGS) node.o graph.o parser.o jsoncpp.o config.o -o main
parser.o: parser.cpp
	$(CC) $(CCFLAGS) -c parser.cpp
node.o: node.cpp
	$(CC) $(CCFLAGS) -c node.cpp
graph.o: graph.cpp graph.hpp
	$(CC) $(CCFLAGS) -c graph.cpp
config.o: config.cpp config.hpp
	$(CC) $(CCFLAGS) -c config.cpp
jsoncpp.o: jsoncpp.cpp json/json.h
	$(CC) $(CCFLAGS) -c jsoncpp.cpp

clean:
	rm -Rf *~ node.o graph.o parser.o config.o main
