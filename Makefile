CC = g++
CFLAGS = -Wall -std=c++17 -O2 -pthread

# test

test: test1 test2 test3
	./test1
	./test2
	./test3

test1: test1.cpp
	$(CC) $(CFLAGS) -o test1 test1.cpp

test2: test2.cpp
	$(CC) $(CFLAGS) -o test2 test2.cpp

test3: test3.cpp
	$(CC) $(CFLAGS) -fopenmp -o test3 test3.cpp

clean:
	rm -f *.o test test1 test2 test3
