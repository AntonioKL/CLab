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
	/*We are taking more than nessacry - 100 instead, since we were told we can assume the input is valid with length of each line*/
	char lineString[MAX_LINE_LENGTH];
	while (fgets(lineString, MAX_LINE_LENGTH, file))
	{
		if (runStatus -> lineCount >= MAX_FILE_LINES)
		{
			printf("ERROR: File is too long, Maximum allowed line numbers is %d\n", MAX_FILE_LINES);
			runStatus -> errNum++; 
			return runStatus -> errNum;
		}

		runStatus -> line = lineString;
		runStatus -> originalLine = lineString;
		runStatus -> lineCount++;
		lineProccessor(runStatus, file);
	}
	
	return runStatus -> errNum;
}

void lineProccessor(RunStatus *runStatus, FILE *file)
{
	char labelContent[MAX_TAG_LEN] = "\0";

	if (!( isLineEmpty(runStatus) || isLineComment(runStatus) ))
	{
		
		getLabel(runStatus, labelContent);
		
		if(*labelContent)
		{
			printf("szzzzzz%s777\n", labelContent);
		}
	}
	
/*
		Check IF Directive
		Parse it
		Parse Command



*/	

}


