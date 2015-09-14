.PHONY: clean

binaries=mesycut

CXX = g++ -O2 -Wall -Wno-unused-result

all: clean $(binaries)

mesycut: mesycut.cpp
	$(CXX) $^ -o $@

clean:
	rm -f $(binaries) *.o

install:
	echo "install mesycut to /usr/local/bin"
	sudo cp mesycut /usr/local/bin
