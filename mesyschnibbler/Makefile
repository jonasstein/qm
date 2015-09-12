.PHONY: clean

binaries=mesyschnibbler

CXX = g++ -O2 -Wall -Wno-unused-result

all: clean $(binaries)

mesyschnibbler: mesyschnibbler.cpp
	$(CXX) $^ -o $@

clean:
	rm -f $(binaries) *.o
