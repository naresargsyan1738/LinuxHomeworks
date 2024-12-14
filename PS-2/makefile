all: interactive-shell

interactive-shell: main.o
	g++ -Wall -Wextra -Werror main.o -o interactive-shell

main.o: main.cpp
	g++ -Wall -Wextra -Werror -c main.cpp -o main.o

clean:
	rm -f main.o interactive-shell
