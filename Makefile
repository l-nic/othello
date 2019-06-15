.PHONY: clean debug run

CXX = g++
CXXFLAGS = -std=c++11 -march=native -O3 -mavx2 -fopt-info-vec -ftree-vectorize -fopt-info-vec-missed
LDFLAGS =

SOURCES = othello.cpp main.cpp
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
	$(CXX) $(CXXFLAGS) -pg -o othello.debug $(SOURCES)

run:
	./$(EXECUTABLE)
