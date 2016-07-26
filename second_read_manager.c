/*
=====================================================================================================================
SecondReadManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the Second Read of Asemmbler File
=====================================================================================================================
*/

#include "main_header.h"

int SecondReadManager(RunStatus *runStatus, MemoryDump *memStatus)
{
	int i = 0;

	checkEntryLabels(runStatus);
	for (i = 0; i < runStatus -> lineCount; i++)
	{
		runStatus -> errNum += dumpLine(i, memStatus, runStatus);
	}

	for (i = 0; i < runStatus -> dataCount; i++)
	{
		runStatus -> errNum += dumpData(runStatus -> dataArray[i], memStatus);
	}
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
				runStatus -> entryArray[i].memAddress = runStatus -> finalLabelArray[j].memAddress;
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

int dumpLine(int lineNum, MemoryDump *memStatus, RunStatus *runStatus)
{
	int errNum = 0;
	Lines line = runStatus -> lineArray[lineNum];
	Operand *op1 = line.op1;
	Operand *op2 = line.op2;
	Label *labels = runStatus -> finalLabelArray;
	
	

	if ( line.cmdId >= 0 )
	{
		if ( updateOperandLabelAddress(op1, runStatus, lineNum)  && updateOperandLabelAddress(op2, runStatus, lineNum))
		{
			printf ("sss%d\n", line.cmdId);
		}
		else
		{
			errNum +=1;
		}
		
	}
	return 0;

}

int dumpData(int dataArray,  MemoryDump *memStatus)
{
	return 0;
}

int updateOperandLabelAddress(Operand *op, RunStatus *runStatus, int lineNum)
{
	printf ("www---%s---wwww\n", op->str);
	if (op -> type == DIRECT)
	{
		if (getLabelAddress(runStatus , op) > -1)
		{
			printf ("ssss---%s---ssss\n", op->str);
		}
		else
		{
			printf ("zzzzzzz\n");
		}
	}
	else if ( op -> type == DYNAMIC)
	{
		printf ("aa--%s--aa\n", op->str);
	}
	
	return TRUE;
}


int getLabelAddress(RunStatus *runStatus, Operand *op)
{
	char * str = op -> label;
	int i;

	for ( i = 0; i < runStatus -> finalLabelCount; i++)
	{
		if (!strcmp(str, runStatus -> finalLabelArray[i].name ))
		{
			return runStatus -> finalLabelArray[i].memAddress;
		}
	}

	for ( i=0; i < runStatus -> externCount; i++)
		{
			if (!strcmp(str, runStatus -> externArray[i].name))
			{
				addExternFile(runStatus, str, op -> memAddress );
				
			}
		}
	

	return -1;
	
}


