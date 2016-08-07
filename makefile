assembler: assembler_compiler.o file_proccess.o first_read_manager.o global_functions.o second_read_manager.o file_output_manager.o
	gcc -Wall -ansi -pedantic assembler_compiler.o file_proccess.o first_read_manager.o global_functions.o second_read_manager.o file_output_manager.o -o assembler -lm

assembler_compiler.o: assembler_compiler.c
	gcc -Wall -ansi -pedantic -c assembler_compiler.c -lm

file_proccess.o: file_proccess.c
	gcc -Wall -ansi -pedantic -c file_proccess.c -lm

first_read_manager.o: first_read_manager.c
	gcc -Wall -ansi -pedantic -c first_read_manager.c -lm

second_read_manager.o: second_read_manager.c
	gcc -Wall -ansi -pedantic -c second_read_manager.c -lm

global_functions.o: global_functions.c
	gcc -Wall -ansi -pedantic -c global_functions.c -lm

file_output_manager.o: file_output_manager.c
	gcc -Wall -ansi -pedantic -c file_output_manager.c -lm

clean:
	rm -f assembler
	rm -f *.o


