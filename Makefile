CC = g++
CFLAGS = -Wall -std=c++17 -O2 -pthread
OBJS = bucket.o bucket_chain.o hash_table.o

# objs

bucket.o : bucket.cpp
	$(CC) $(CFLAGS) -c bucket.cpp

bucket_chain.o : bucket_chain.cpp
	$(CC) $(CFLAGS) -c  bucket_chain.cpp

hash_table.o : hash_table.cpp
	$(CC) $(CFLAGS) -c hash_table.cpp

# test

test: test1 test2 test3
	./test1
	./test2
	./test3

test1: $(OBJS) test1.o
	$(CC) $(CFLAGS) -o test1 $(OBJS) test1.o

test2: $(OBJS) test2.o
	$(CC) $(CFLAGS) -o test2 $(OBJS) test2.o

test3: $(OBJS) test3.o
	$(CC) $(CFLAGS) -fopenmp -o test3 $(OBJS) test3.o

test1.o : test1.cpp
	$(CC) $(CFLAGS) -c test1.cpp

test2.o : test2.cpp
	$(CC) $(CFLAGS) -c test2.cpp

test3.o : test3.cpp
	$(CC) $(CFLAGS) -fopenmp -c test3.cpp

clean:
	rm -f *.o test test1 test2 test3
