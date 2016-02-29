CC=g++
LFLAGS= -Wall -g -lm -lrational
CFLAGS= -c -g

all: main

main: main.o librational.a
	$(CC)  main.o -L . $(LFLAGS) -o test
	
main.o: main.cpp
	$(CC) main.cpp $(CFLAGS)

librational.a: number.o rational.o bvector.o
	rm -f $@
	ar cqs $@ number.o rational.o bvector.o

number.o: number.cpp
	$(CC) number.cpp $(CFLAGS)

rational.o: rational.cpp
	$(CC) rational.cpp $(CFLAGS)

bvector.o: bvector.cpp
	$(CC) bvector.cpp $(CFLAGS)
clean:
	rm -rf *o main
