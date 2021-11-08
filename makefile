all: main.o
	gcc -o main main.o

main.o:
	gcc -c main.c

clean:
	rm -rf main main.o nyc_pop.txt

run:
	./main