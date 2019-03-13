all: main


main: main.o 
	g++ -o $@ $^ -L../src -lcore

main.o: main.cpp
	g++ -c $^ -I../src

clean:
	rm *.o main