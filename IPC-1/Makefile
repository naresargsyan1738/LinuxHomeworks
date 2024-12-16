all: signal-echo

signal-echo: main.o
	g++ -Wall -Wextra -Werror main.o -o signal-echo

main.o: main.cpp
	g++ -Wall -Wextra -Werror -c main.cpp -o main.o

clean:
	rm -f main.o signal-echo
