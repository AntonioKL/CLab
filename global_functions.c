/*
=====================================================================================================================
global_functions
	Author: Anton Kuligin 
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
		printf("ERROR: Line #%d, Invalid Label Name - Label should start with Letter at the first collumn\n", runStatus -> lineCount);
		return ;
	}
	
	while (!isspace(*c) && *c != '\n' && *c != ':')
	{
		if (! isalnum(*c))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Label should contain only Letters and Numbers\n", runStatus -> lineCount);
			return ;
		}
		i++;
		c++;
	}

	if (isspace(*c) || *c == '\n')
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should contain only one word, without spaces\n", runStatus -> lineCount);
		return ;
	}
	if (*c == ':')
	{
		strncpy(temp_label, runStatus -> originalLine,i); /*We don't want to copy the : itself*/
		i++;
        	runStatus -> line += i;
		strcpy(label, temp_label);
		printf("%s", runStatus -> line);
	}


}




