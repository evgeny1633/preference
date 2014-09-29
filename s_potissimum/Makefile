CC=g++
CFLAGS=-c -Wall -I/home/kondratyuk/programms/boost_1_56_0 -L/home/kondratyuk/programms/boost_1_56_0/stage/lib
LDFLAGS=-lboost_system -lboost_thread -lpthread 
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
