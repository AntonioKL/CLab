/*
=====================================================================================================================
FirstReadManager
	Author: Anton Kuligin 
	Description: Handles the First Read of Aseembler File
=====================================================================================================================
*/

#include "main_header.h"

int firstReadManager(RunStatus *runStatus, FILE *file)
{
	int linesFound = 0;
	/*We are taking more than nessacry - 100 instead, since we were told we can assume the input is valid with length of each line*/
	char lineString[MAX_LINE_LENGTH];
	while (fgets(lineString, MAX_LINE_LENGTH, file))
	{
		if (linesFound >= MAX_FILE_LINES)
		{
			printf("ERROR: File is too long, Maximum allowed line numbers is %d", MAX_FILE_LINES);
			runStatus -> errNum++; 
			return runStatus -> errNum;
		}

		runStatus -> line = lineString;
		runStatus -> lc++;
		lineProccessor(runStatus, file);
		linesFound++;
	}
	
	return runStatus -> errNum;
}

void lineProccessor(RunStatus *runStatus, FILE *file)
{
	int flag_label = 0;
	printf("eeeee%d\n", flag_label);
}


