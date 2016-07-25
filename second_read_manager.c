/*
=====================================================================================================================
SecondReadManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the Second Read of Asemmbler File
=====================================================================================================================
*/

#include "main_header.h"

int SecondReadManager(RunStatus *runStatus)
{
	checkEntryLabels(runStatus);
	return runStatus -> errNum;
}

void checkEntryLabels(RunStatus *runStatus)
{
	int i,j;
	int flagRun = TRUE;

	for (i=0; i< runStatus -> entryCount ; i++)
	{
		for ( j=0; j < runStatus -> finalLabelCount && flagRun; j++)
		{
			if (!strcmp(runStatus -> entryArray[i].name, runStatus -> finalLabelArray[j].name ))
			{
				flagRun = FALSE;
				runStatus -> entryArray[i].mem_address = runStatus -> finalLabelArray[j].mem_address;
			}
		}
		if (flagRun)
		{
			printf("ERROR: Invalid Entry Label - Label \"%s\" does not exists in the programm.\n", runStatus -> entryArray[i].name);
			runStatus -> errNum ++;
		}
		flagRun = TRUE;
	}

}








