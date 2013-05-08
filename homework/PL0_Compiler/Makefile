pl0_compiler: main.o Lexical.o parsing.o
	gcc -o pl0_compiler main.o Lexical.o parsing.o

main.o: main.c pl0.h
	gcc -c main.c
Lexical.o: Lexical.c pl0.h
	gcc -c Lexical.c
parsing.o: parsing.c pl0.h
	gcc -c parsing.c
clean:
	rm *.o
