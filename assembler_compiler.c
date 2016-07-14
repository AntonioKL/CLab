/*
=====================================================================================================================
Main File
	Author: Anton Kuligin 323206730
	Description: Simulation of an assembler with an output in unique 8 base encoding.
	Input : File names to convert without .as extensions
	Output :  1) Syntax Errors and Warnings
		  2) For valid file we will create several files with the following extensions:
			- .ob: encoded operation for linker
			- .ext: memory address of external variable
			- .ent: memory address of the labels

=====================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*The main complier function that will run on providd files*/

int main(int argc, char* argv[])
{
	int termination_flag = 0	
	FILE* proccessed_file;
	
	if (argc == 1)
	{
		/*ERROR - No Files*/
		termination_flag = 1
	}
	else
	{

	}

	/* Initialize operation list 
	if (!init_operation_list()) {
		free_operation_list();
		print_runtime_error("Could not initializes operation definitions");
		exit(0);
	}*/

	return 0;
}






