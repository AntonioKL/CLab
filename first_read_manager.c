/*
=====================================================================================================================
FirstReadManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
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
			printf("ERROR: File is too long, Maximum allowed line numbers is %d, Aborting Run\n", MAX_FILE_LINES);
			runStatus -> errNum++; 
			return runStatus -> errNum;
		}

		runStatus -> line = lineString;
		runStatus -> originalLine = lineString;
		runStatus -> lineCount++;
		lineProccessor(runStatus);
	}
	
	return runStatus -> errNum;
}

void lineProccessor(RunStatus *runStatus)
{
	char labelContent[MAX_TAG_LEN] = "\0";
	int i = 0;
	char *arr_labelName;

	if (!( isLineEmpty(runStatus) || isLineComment(runStatus) ))
	{
		
		getLabel(runStatus, labelContent);
		
		if(*labelContent)
		{
			for (i = 0; (runStatus -> labelCount) > i; i++) /*Chekcing for duplicating tag names*/
			{
				arr_labelName = runStatus -> labelArray[i].name;

				if (! strcmp (arr_labelName, labelContent) )
				{
					printf("ERROR: Line #%d, Invalid Label Name - Label %s is in use already\n", runStatus -> lineCount, labelContent);
					runStatus -> errNum ++;
					return ;
				}
			}	
		}

		skipSpaces(runStatus);

		if ( *(runStatus->line) == '.')
		{
			directiveManager(runStatus);
		}
		firstParseCmd(runStatus, labelContent);
	}
}


void firstParseCmd(RunStatus *runStatus, char *label)
{
	return ;
}

void directiveManager(RunStatus *runStatus)
{
	return ;
}
