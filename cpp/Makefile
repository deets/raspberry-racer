CXXFLAGS = -g
CC := g++ $(CXXFLAGS)

all: test


clean:
	rm test
	rm *.o

test:	terminal.o test.o 
	$(CC) -o test test.o terminal.o


test.o:	test.cc
	$(CC) -c test.cc

terminal.o: terminal.cc terminal.hh
	$(CC) -c terminal.cc