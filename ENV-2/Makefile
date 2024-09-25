all: program

program: main.o Complex.o Sorting.o
	g++ -o program main.o Complex.o Sorting.o

main.o: main.cpp Complex.h Sorting.h
	g++ -c main.cpp -o main.o

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp -o Complex.o

Sorting.o: Sorting.cpp Sorting.h
	g++ -c Sorting.cpp -o Sorting.o

clean:
	rm -f main.o Complex.o Sorting.o program
