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
        	return 1;
   	}
 	return 0;
}

int isLineComment(RunStatus *runStatus)
{
	if (*(runStatus -> line) == ';')
	{
		return 1;
	}
	return 0;
		
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


int isLineWithTag(RunStatus *runStatus)
{
	return 1; /*****************FIX IT !!!!****************/
}





