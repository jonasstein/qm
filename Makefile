all: mesyread

mesyread: mesyread.cpp
	g++ mesyread.cpp -o mesyread

clean:
	rm mesyread a.out
