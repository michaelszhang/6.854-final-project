all: main clean

binary_heap.o: binary_heap.cc
	g++ -Wall -Wextra -std=c++14 -O2 -Wshadow -c -o binary_heap.o binary_heap.cc

item.o: item.cc
	g++ -Wall -Wextra -std=c++14 -O2 -Wshadow -c -o item.o item.cc

main.o: main.cc
	g++ -Wall -Wextra -std=c++14 -O2 -Wshadow -c -o main.o main.cc

tests.o: tests.cc
	g++ -Wall -Wextra -std=c++14 -O2 -Wshadow -c -o tests.o tests.cc

main: main.o binary_heap.o item.o tests.o
	g++ -o main main.o binary_heap.o item.o tests.o

clean:
	rm -rf *.o

.PHONY: all clean
