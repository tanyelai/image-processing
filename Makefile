all: pgm.o
	gcc -Wall main.c -o main pgm.o -lm

pgm.o: pgm.c
	gcc -c -Wall pgm.c 

clean:
	rm *.o main *.pgm

clean-test:
	rm test