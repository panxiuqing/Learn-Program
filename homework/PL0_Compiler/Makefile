pl0_compiler: main.o Lexical.o
	gcc -o pl0_compiler main.o Lexical.o

main.o: main.c pl0.h
	gcc -c main.c
Lexical.o: Lexical.c pl0.h
	gcc -c Lexical.c
clean:
	rm *.o