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
		dumpLine(i, memStatus, runStatus);
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

void dumpLine(int lineNum, MemoryDump *memStatus, RunStatus *runStatus)
{
	Lines line = runStatus -> lineArray[lineNum];
	Operand *op1 = line.op1;
	Operand *op2 = line.op2;
	Label *labels = runStatus -> finalLabelArray;
	
	

	if ( line.cmdId >= 0 )
	{
		if ( updateOperandLabelAddress(op1, runStatus, lineNum)
  && updateOperandLabelAddress(op2, runStatus, lineNum))
		{
			printf ("sss%d\n", line.cmdId);
		}
			
	}

}

int dumpData(int dataArray,  MemoryDump *memStatus)
{
	return 0;
}

int updateOperandLabelAddress(Operand *op, RunStatus *runStatus, int lineNum)
{
	int labelAddress;
	int dataAddress;
	unsigned int startValueData;

	if (op -> type == DIRECT)
	{
		labelAddress = getLabelAddress(runStatus , op, 1);
		if (labelAddress == -1)
		{
			printf("ERROR: Line #%d, Label \"%s\" doesn't exist.\n", lineNum, op->str);
			runStatus -> errNum ++;
			return FALSE;
		}
	}
	else if ( op -> type == DYNAMIC)
	{
		labelAddress = getLabelAddress(runStatus , op, 0);
		if (labelAddress != -1)
		{
			if (runStatus -> finalLabelArray[labelAddress].isData)
			{
				dataAddress = runStatus -> finalLabelArray[labelAddress].memAddress - runStatus -> ic - FIRST_MEM_ADDR;
				startValueData = runStatus -> dataArray[dataAddress];
			}
			else
			{
				startValueData = getIntFromWord(runStatus -> finalLabelArray[labelAddress].word);
			}
			
			op -> val = getRequiredBitsFromLabel(startValueData, op -> up, op-> down);
		}
		else 
		{
			printf("ERROR: Line #%d, Label \"%s\" doesn't exist.\n", lineNum, op->str);
			runStatus -> errNum ++;
			return FALSE;
		}
	}
	
	return TRUE;
}


int getLabelAddress(RunStatus *runStatus, Operand *op, int checkExtern)
{
	char *str = op -> label;
	int i;

	for ( i = 0; i < runStatus -> finalLabelCount; i++)
	{
		if (!strcmp(str, runStatus -> finalLabelArray[i].name ))
		{
			op -> val = runStatus -> finalLabelArray[i].memAddress;
			return i;
		}
	}

	for ( i = 0; i < runStatus -> externCount && checkExtern; i++)
		{
			if (!strcmp(str, runStatus -> externArray[i].name))
			{
				op -> val = 1;
				addExternFile(runStatus, str, op -> memAddress);
				return i;
			}
		}

	return -1;
	
}

int getRequiredBitsFromLabel(int val, int up , int down)
{
	int diff = (up - down) + 1;
	int lsb = 1;
	int i = 0;
	int tmp = 1;
	int tmp2 = -1;
	

	if (val == 0) 
	{
		return 0;
	}

	lsb <<= up;
	
	if ( val != (val | lsb))
	{
		val >>= down;
		while (i < diff-1)
		{
			tmp = (tmp << 1) + 1;
			i++;
		}
		return (tmp & val);
	}
	else
	{
		val >>= down;
		i = 0;
		while (i < diff)
		{
			tmp = (tmp << 1) + 1;
			i++;
		}
		tmp2 <<= diff;
		return (tmp2 | (tmp & val ));
	}

}










