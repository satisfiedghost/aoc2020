.PHONY: clean
CXX=g++
CFLAGS=-Wall -Wextra -std=c++17


all: main.o
	$(CXX) -o puzzle $^

main.o: main.cpp
	$(CXX) -c -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o puzzle