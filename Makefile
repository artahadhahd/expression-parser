CC=g++
CXX_STANDARD=c++17
INCLUDE=-I.
CXXWARNINGS=-Wall -Wextra -Wpedantic
CXXFLAGS=$(CXXWARNINGS) -g $(INCLUDE) -std=$(CXX_STANDARD)
BINDIR=.
BIN=lcc
DIRS=.
CXX_FILES=$(foreach D,$(DIRS),$(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(CXX_FILES))

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $^

clean:
	rm $(OBJECTS)

release: # optimized
	$(CC) $(INCLUDE) $(CXX_FILES) -O3  -o bin/lcc
