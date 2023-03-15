CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: main.o cachesimfuncs.o
	$(CC) -o csim main.o cachesimfuncs.o

main.o: main.cpp cachesimfuncs.h
	$(CC) $(CFLAGS) -c main.cpp

cachesimfuncs.o: cachesimfuncs.cpp cachesimfuncs.h
	$(CC) $(CFLAGS) -c cachesimfuncs.cpp

.PHONY: clean all

solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp

clean:
	rm -f *.o csim