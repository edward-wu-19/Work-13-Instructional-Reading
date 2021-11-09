all: main.o
	gcc -o main main.o

main.o:
	gcc -c main.c

clean:
	rm -rf main main.o nyc_pop.txt

run:
	./main

# below is for debugging/testing purposes
# remake:
# 	rm -rf main main.o nyc_pop.txt
# 	gcc -c main.c
# 	gcc -o main main.o
# 	./main -read_csv
# 	./main -read_data

read:
	./main -read_data