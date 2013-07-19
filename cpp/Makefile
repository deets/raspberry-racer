CXXFLAGS:=-g -Igmock-1.6.0/include -Igtest-1.6.0/include
LDFLAGS=-Lgmock-1.6.0/lib/.libs
CC := g++ $(CXXFLAGS)
GTEST_OBJECTS = gtest-1.6.0/lib/.libs/libgtest.a gmock-1.6.0/lib/.libs/libgmock.a


all: test

clean:
	rm test
	rm *.o

test:	test.o terminal.o
	$(CC) $(LDFLAGS) -o test $(GTEST_OBJECTS) test.o terminal.o


test.o:	test.cc terminal.hh
	$(CC) $(CXXFLAGS) -c test.cc


terminal.o:	terminal.cc terminal.hh
	$(CC) -c terminal.cc