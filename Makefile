all:
	gcc -o main main.c -lpthread 
run:
	./main
debug:
	gcc  -g3 ./*.c -o main
fullDebug :
	gcc  -Wall -g3 ./*.c -o main
	