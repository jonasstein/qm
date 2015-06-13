all: mesyread

mesyread: mesyread.cpp
	g++ mesyread.cpp -o mesyread

clean:
	rm -f mesyread a.out
