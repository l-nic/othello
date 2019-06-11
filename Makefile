.PHONY: clean debug run

CXX = g++
CXXFLAGS = -std=c++11 -march=native -O3
LDFLAGS =

SOURCES = board.cpp othello.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = othello

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) othello.debug

debug:
	$(CXX) $(CXXFLAGS) -g -o othello.debug $(SOURCES)

run:
	./$(EXECUTABLE)
