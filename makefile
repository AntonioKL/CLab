assembler: assembler_compiler.o file_proccess.o first_read_manager.o global_functions.o 
	gcc -Wall -ansi -pedantic assembler_compiler.o file_proccess.o first_read_manager.o global_functions.o -o assembler
	rm *.o

assembler_compiler.o: assembler_compiler.c
	gcc -Wall -ansi -pedantic -c assembler_compiler.c

file_proccess.o: file_proccess.c
	gcc -Wall -ansi -pedantic -c file_proccess.c

first_read_manager.o: first_read_manager.c
	gcc -Wall -ansi -pedantic -c first_read_manager.c

global_functions.o: global_functions.c
	gcc -Wall -ansi -pedantic -c global_functions.c

clean:
	rm *.o

git:
	git status
	git add -A
	git commit
	git push origin master

