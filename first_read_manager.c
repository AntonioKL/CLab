/*
=====================================================================================================================
FirstReadManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the First Read of Aseembler File
=====================================================================================================================
*/

#include "main_header.h"


const Directive globalDirective[] = 
/* Name 	| 	Parse Function */
{	
	{ ".data", parseDataDirective } ,
	{ ".string", parseStringDirective } ,
	{ ".entry", parseEntryDirective },
	{ ".extern", parseExternDirective },
	{ NULL } /*Null will be used as an end of the array*/
};

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
					printf("ERROR: Line #%d, Invalid Label Name - Label Name \"%s\" is in use already\n", runStatus -> lineCount, labelContent);
					runStatus -> errNum ++;
					return ;
				}
			}	
		}

		skipSpaces(runStatus);

		if ( *(runStatus->line) == '.')
		{
			scanDirective(runStatus, labelContent);
		}
		else
		{
			firstParseCmd(runStatus, labelContent);
		}
	}
}


void firstParseCmd(RunStatus *runStatus, char *label)
{
	printf("Parse\n");
}

void scanDirective(RunStatus *runStatus, char *label)
{
	char directive[MAX_LINE_LENGTH];
	char *c = runStatus -> line;
	int i = 0;
	
	while (!isspace(*c) && *c != '\n')
	{
		directive[i] = *c;
		i++;
		c++;
	}
	directive[i] = '\0';
	runStatus-> line = runStatus-> line + strlen(directive);
	skipSpaces(runStatus);
	
	i = 0;
	while ( globalDirective[i].name )
	{
		if (!strcmp(directive, globalDirective[i].name))
		{
			globalDirective[i].parsingFunc(runStatus, label);
			return;
		}
		i++;
	}
	printf("ERROR: Line #%d, Invalid Directive Name - Directive %s is not defined.\n", runStatus -> lineCount, directive);
	runStatus -> errNum ++;
	
}

void parseDataDirective(RunStatus *runStatus, char *label)
{
	int arrNum[MAX_LINE_LENGTH];
	int number;	

	int runningStateFlag = TRUE;
	int numberStateFlag = TRUE;
	int commaStateFlag = FALSE; 
	int cmdStatus = FALSE;

	int i = 0;
	int dataCounter = 0;
	char *directive = ".data";


	if (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n')
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s is empty.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}
	while (runningStateFlag)
	{
		skipSpaces(runStatus);

		if (numberStateFlag)
		{
			cmdStatus = sscanf(runStatus -> line, "%d", &number);

			if (!cmdStatus)
			{
				printf("ERROR: Line #%d, Invalid Directive Data - Not valid data, should be a set of numbers (positive/negative) separated by commas.\n", runStatus -> lineCount);
				runStatus -> errNum ++;
				return ;
			}
			arrNum[dataCounter++] = number;

			if (*(runStatus -> line) == '+' || *(runStatus -> line) == '-')
			{
				runStatus -> line ++;
			}
			while (isdigit(*(runStatus -> line)))
			{
				runStatus -> line ++;
			}
			numberStateFlag = FALSE;
			commaStateFlag = TRUE; 

		}
		if (commaStateFlag)
		{
			skipSpaces(runStatus);
			if (! ( *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' || *(runStatus -> line) == ','))
			{
				printf("ERROR: Line #%d, Invalid Directive Data - Not valid data, should be a set of numbers (positive/negative) separated by commas.\n", runStatus -> lineCount);
				runStatus -> errNum ++;
				return ;
			}
			if (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n')
			{
				runningStateFlag = FALSE;
			}
			else
			{
				runStatus -> line ++;
				numberStateFlag = TRUE;
				commaStateFlag = FALSE; 
			}
		}
	
	}
	if (*label && label)
	{
		addLabel(runStatus, label);
	}
	while(i<dataCounter)
	{
		addDirData(runStatus, arrNum[i]);
		i++;
	}
}


void parseStringDirective(RunStatus *runStatus, char *label)
{
	char arrChar[MAX_LINE_LENGTH];
	int dataCounter = 0;
	int i = 0;
	
	char *directive = ".string";

	skipSpaces(runStatus);

	if (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' )
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s is empty.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}
	if ( *(runStatus -> line) == '"' )
	{
		runStatus -> line ++;

		while (! (*(runStatus -> line) == '"' || *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' ))
		{
			arrChar[dataCounter++] = *(runStatus -> line);
			runStatus -> line ++;
			if ( *(runStatus -> line) == '\\' )
			{
				arrChar[dataCounter++] = *(runStatus -> line);
				runStatus -> line ++;
				if ( *(runStatus -> line) == '"' )
				{
					arrChar[dataCounter++] = *(runStatus -> line);
					runStatus -> line ++;
				}
			}
		}

		if ( *(runStatus -> line) != '"' )
		{
			printf("ERROR: Line #%d, Invalid Directive Data - Not valid data, should hold a string, between the \" chars\n", runStatus -> lineCount);
				runStatus -> errNum ++;
				return ;
		}
		runStatus -> line ++;
		skipSpaces(runStatus);

		if (! (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' ))
		{
			printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s doesn't hold a valid string.\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
		}
		
	}
	else
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s doesn't hold a valid string, string should start with \" .\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
	}

	if (*label && label)
	{
		addLabel(runStatus, label);
	}
	while(i<dataCounter)
	{
		addDirData(runStatus, arrChar[i]);
		i++;
	}
	
	addDirData(runStatus, '\0');

}


void parseExternDirective(RunStatus *runStatus, char *label)
{
	char labelContent[MAX_TAG_LEN] = "\0";
	int i = 0;

	char *directive = ".extern";

	skipSpaces(runStatus);
	if (*label && label)
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s cannot have a label .\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
	}

	if (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' )
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s is empty.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}
	
	getLabelReference(runStatus, labelContent);

	if(! ( *labelContent ))
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s wrong Label Reference.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}
	
	while (i <= runStatus -> entryCount)
	{
		if (runStatus -> entryArray && ! strcmp (runStatus -> entryArray[i].name, labelContent))
		{
			printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s label defined  as .entry already.\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
		}
		i++;
	}
	i = 0;
	while (i <= runStatus -> externCount)
	{
		if (runStatus -> externArray && ! strcmp (runStatus -> externArray[i].name, labelContent))
		{
			printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s label defined  as .extern already.\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
		}
		i++;
	}
	
	addExternDir(runStatus, labelContent);
}


void parseEntryDirective(RunStatus *runStatus, char *label)
{
	char labelContent[MAX_TAG_LEN] = "\0";
	int i = 0;

	char *directive = ".entry";

	skipSpaces(runStatus);

	if (*label && label)
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s cannot have a label .\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
	}

	if (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' )
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s is empty.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}

	getLabelReference(runStatus, labelContent);
	if(! ( *labelContent ))
	{
		printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s wrong Label Reference.\n", runStatus -> lineCount, directive);
		runStatus -> errNum ++;
		return ;
	}
	
	while (i <= runStatus -> entryCount)
	{
		if (runStatus -> entryArray && ! strcmp (runStatus -> entryArray[i].name, labelContent))
		{
			printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s label defined  as .entry already.\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
		}
		i++;
	}
	i = 0;
	while (i <= runStatus -> externCount)
	{
		if (runStatus -> externArray && ! strcmp (runStatus -> externArray[i].name, labelContent))
		{
			printf("ERROR: Line #%d, Invalid Directive Definition - Directive %s label defined  as .extern already.\n", runStatus -> lineCount, directive);
			runStatus -> errNum ++;
			return ;
		}
		i++;
	}

	addEntryDir(runStatus, labelContent);
}




