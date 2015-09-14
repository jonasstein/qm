.PHONY: clean

binaries=mesyread

CXX = g++ -O2 -Wall -Wno-unused-result

all: clean $(binaries)

mesyread: mesyread.cpp
	$(CXX) $^ -o $@

clean:
	rm -f $(binaries) *.o

