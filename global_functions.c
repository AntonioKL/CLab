/*
=====================================================================================================================
global_functions
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Stores functions that used in many modules
=====================================================================================================================
*/

#include "main_header.h"




int isLineEmpty(RunStatus *runStatus)
{
	skipSpaces(runStatus);
	if(*(runStatus->line)==EOF || *(runStatus->line)=='\n')
	{
        	return TRUE;
   	}
 	return FALSE;
}

int isLineComment(RunStatus *runStatus)
{
	if (*(runStatus -> originalLine) == ';')
	{
		return TRUE;
	}
	return FALSE;
		
}

char *getCharAfterSpace(char *p)
{
	while(isspace(*p))
	{
		if (*p == EOF || *p== '\n')
		{
			return p;
		}
		p++;
	}

	return p;
}

void skipSpaces(RunStatus *runStatus)
{
	runStatus -> line = getCharAfterSpace(runStatus -> line);
}


void getLabel(RunStatus *runStatus, char *label)
{
	char temp_label[MAX_TAG_LEN]="\0";

	int i = 0; /*Counter for the label Chars*/
	char *c = runStatus-> originalLine;

	
	if (! strchr(runStatus-> originalLine, ':') )
		return ; /*It is not a label*/

	if (!isalpha(*c))
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should start with Letter at the first collumn.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		return ;
	}
	
	while (!isspace(*c) && *c != '\n' && *c != ':')
	{
		if (! isalnum(*c))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Label should contain only Letters and Numbers.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			return ;
		}
		i++;
		c++;
	}

	if (isspace(*c) || *c == '\n')
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should contain only one word, without spaces.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		return ;
	}
	if (*c == ':')
	{
		if (i >= MAX_TAG_LEN)
		{
			printf("ERROR: Line #%d, Invalid Label Name - Label should contain no more than %d chars.\n", runStatus -> lineCount, MAX_TAG_LEN);
			runStatus -> errNum ++;
			return ;
		}
		strncpy(temp_label, runStatus -> originalLine,i); /*We don't want to copy the ":" itself*/
		if(isRegister(temp_label))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Illegal Name , You cannot use Register Name.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			return ;
		}
		i++;
        	runStatus -> line += i;
		strcpy(label, temp_label);
	}


}

int isRegister(char *str)
{
	if (str[0] == 'r'  && str[1] >= '0' && str[1] - '0' <= MAX_REGISTERS && str[2] == '\0') 
	{
		return TRUE;
	}
	return FALSE;
}

void addLabel(RunStatus *runStatus, char *label)
{
	printf("1%s1\n", label);
}
void addDirective(RunStatus *runStatus, int num)
{
	return ;
}


