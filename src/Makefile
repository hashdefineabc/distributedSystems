CC := g++
CXX := g++

CFLAGS := -Wall -std=c++11
LFLAGS := -pthread

all: client server

client: ClientMain.cpp serialization.cpp
	$(CXX) -o client ClientMain.cpp serialization.cpp $(CFLAGS) $(LFLAGS)

server: ServerMain.cpp serialization.cpp
	$(CXX) -o server ServerMain.cpp serialization.cpp $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	rm -rf client server
