all: program

program: main.o
	g++ -Wall -Wextra -Werror main.o -o program
main.o: main.cpp
	g++ -Wall -Wextra -Werror main.cpp -o main.o
clean:
	rm -f main.o program
