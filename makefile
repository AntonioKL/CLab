compiler: assembler_compiler.o assembler_compiler.o
	gcc -Wall -ansi -pedantic assembler_compiler.o -o compiler
assembler_compiler.o: assembler_compiler.c
	gcc -Wall -ansi -pedantic -c assembler_compiler.c 

clean:
	rm *.o
