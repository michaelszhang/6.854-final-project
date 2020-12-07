all: main

main:
	g++ -Wall -Wextra -std=c++14 -O2 -Wshadow -o main main.cc

clean:
	rm -rf main *.o

.PHONY: all clean
