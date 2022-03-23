CC = gcc

all: part3 sine_tester

part3.o: part3.c
	gcc -c part3.c -o part3.o

sine_tester.o: sine_tester.c
	gcc -c sine_tester.c -o sine_tester.o

part3: part3.o
	gcc part3.o -o part3

sine_tester: sine_tester.o
	gcc sine_tester.o -o sine_tester

clean:
	rm -f part3 sine_tester