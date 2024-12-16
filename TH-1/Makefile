all: array-summary

array-summary: main.o
	g++ -Wall -Wextra -Werror main.o -o array-summary

main.o: main.cpp   
	g++ -Wall -Wextra -Werror -c main.cpp -o main.o

clean:
	rm -f main.o array-summary
