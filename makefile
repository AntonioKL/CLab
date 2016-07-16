Compiler: assembler_compiler.o file_proccess.o
	gcc -Wall -ansi -pedantic assembler_compiler.o file_proccess.o -o Compiler
	rm *.o

assembler_compiler.o: assembler_compiler.c
	gcc -Wall -ansi -pedantic -c assembler_compiler.c

file_proccess.o: file_proccess.c
	gcc -Wall -ansi -pedantic -c file_proccess.c

clean:
	rm *.o


