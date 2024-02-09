CC := g++
CXX := g++

CFLAGS := -Wall -std=c++11
LFLAGS := -pthread

all: client server

client: ClientMain.cpp Socket.cpp
	$(CXX) -o client ClientMain.cpp Socket.cpp $(CFLAGS) $(LFLAGS)

server: ServerMain.cpp Socket.cpp
	$(CXX) -o server ServerMain.cpp Socket.cpp $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	rm -rf client server
