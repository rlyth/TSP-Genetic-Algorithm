CC=g++
CFLAGS=-c -g #-Wall -pedantic-errors
LDFLAGS=
SOURCES = main.cpp Coord.cpp Tour.cpp Population.cpp
HEADERS = Coord.hpp Tour.hpp Population.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = tspGA

all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -f ${OBJECTS} ${EXECUTABLE}
