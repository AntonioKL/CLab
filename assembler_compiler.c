/*
=====================================================================================================================
Main File
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Simulation of an assembler with an output in unique 8 base encoding.
	Input : File names to convert without .as extensions
	Output :  1) Syntax Errors and Warnings
		  2) For valid file we will create several files with the following extensions:
			- .ob: encoded operation for linker
			- .ext: memory address of external variable
			- .ent: memory address of the labels

=====================================================================================================================
*/

#include "main_header.h"

/*
The main complier function that will run on provided files and parse them
Input: Assembler files as command line arguments, without the .as postfix.
Output: 
	1) Syntax Errors and Warnings
	2) For valid file we will create several files with the following 		   extensions:
		- .ob: encoded operation for linker
		- .ext: memory address of external variable
		- .ent: memory address of the labels
*/

int main(int argc, char *argv[])
{
	int i;	
	
	if (argc < MIN_ARGS)
	{
		printf("ERROR: No file name, please provide the script with a file to run on, at least one file.\n");
		return 1;
	}
	
	/*Parse the provided file one by one*/
	for (i=1; i < argc; i++)
	{
		/*Check for provided fileName length*/
		if (strlen(argv[i]) > MAX_FILENAME_LEN)
		{
			printf("ERROR: Maximum allowed filename length is %d.\n", (MAX_FILENAME_LEN));
			return 1;
		}
		
		parseFile(argv[i]);
		/*Printing Divider between Files*/
		printf("--- Done Parsing: \"%s%s\" ---\n\n", argv[i], INPUT_FILE_EXTENSION);
	}

	return 0;
}






