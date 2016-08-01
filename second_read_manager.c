/*
=====================================================================================================================
SecondReadManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the Second Read of Assembler File
=====================================================================================================================
*/

#include "main_header.h"

/*
Function that manages the second run and dumping the commands , variables and data to memoryDump struct
Input: 
	RunStatus struct
	MemoryDump struct
Output: Number of errors during the pass
*/
int SecondReadManager(RunStatus *runStatus, MemoryDump *memStatus)
{
	int i = 0;

	/*Validation that all extern labels exists in the code*/
	checkEntryLabels(runStatus);

	/*We are dumping the command lines first*/
	for (i = 0; i < runStatus -> lineCount; i++)
	{
		dumpLine(i, memStatus, runStatus);
	}
	
	/* After finishing with commands we are dumping the data*/
	for (i = 0; i < runStatus -> dataCount; i++)
	{
		dumpData(runStatus -> dataArray[i], memStatus);
	}

	return runStatus -> errNum;
}

/*
Function that checks if all entry references are valid
Input: 
	RunStatus struct
Output: -
*/
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
			printf("ERROR: Invalid Entry Label - Label \"%s\" does not exists in the program.\n", runStatus -> entryArray[i].name);
			runStatus -> errNum ++;
		}
		flagRun = TRUE;
	}

}

/*
Function that handles the dump of Commands and their operands
Input: 
	integer line Number
	MemoryDump struct
	RunStatus struct
Output: -
*/
void dumpLine(int lineNum, MemoryDump *memStatus, RunStatus *runStatus)
{
	Lines line = runStatus -> lineArray[lineNum];
	Operand *op1 = line.op1;
	Operand *op2 = line.op2;
	WordMemory wordCmd = {0};
	WordMemory wordOp = {0};
	
	/* Check if the line has a command */
	if ( line.cmdId < 0 )
	{
		return ;
	}
	
	/*Updating label information about source operand*/
	updateOperandLabelAddress(op1, runStatus, lineNum);

	/*Updating label information about destination operand*/
	updateOperandLabelAddress(op2, runStatus, lineNum);

	/*Get the word for the line command and add to memoryDump*/
	wordCmd = getCommandWord(runStatus, lineNum);
	addToMem(memStatus, wordCmd);
	
	/*parsing the operands of the command and adding valid operands to memoryDump*/
	if (op1 -> type == REGISTER && op2 -> type == REGISTER)	
	{
		wordOp.eraBits = (eraBit)ABSOLUTE;
		wordOp.wordBits.registerBits.srcReg = op1 -> val;
		wordOp.wordBits.registerBits.dstReg = op2 -> val;
		addToMem(memStatus, wordOp);
	}
	else
	{
		if (op1 -> type != INVAL )
		{
			wordOp = getOperandWord(runStatus, TRUE, op1);
			addToMem(memStatus, wordOp);
		}
		if (op2 -> type != INVAL )
		{
			wordOp = getOperandWord(runStatus, FALSE, op2);
			addToMem(memStatus, wordOp);
		}
	}		
}

/*
Function that handles the dump of the data
Input: 
	integer data
	MemoryDump struct
Output: -
*/
void dumpData(int data,  MemoryDump *memStatus)
{
	unsigned int mask = ~0;

	/*We need only a certain amount of bits all the rest should be 0*/
	mask >>= (BITS_IN_BYTE * sizeof(int) - MEM_WORD_SIZE); 

	if( memStatus -> wordCount >= MAX_DATA_SIZE )
	{
		return ; /*We exceeded the allowed memory size*/
	}

	memStatus -> memArray[memStatus -> wordCount] = mask & data;
	memStatus -> wordCount ++;

}

/*
Function that updates the label of the operand and it's value from table of signs
Input: 
	Operand struct
	RunStatus struct
	integer line Number
Output: True if it was able to update the address , FALSE otherwise
*/
int updateOperandLabelAddress(Operand *op, RunStatus *runStatus, int lineNum)
{
	int labelAddress;
	int dataAddress;
	unsigned int startValueData;

	if (op -> type == DIRECT) /*Label is taken from "tag table"*/
	{
		labelAddress = getLabelAddress(runStatus , op, 1);
		if (labelAddress == -1)
		{
			printf("ERROR: Line #%d, Label \"%s\" doesn't exist.\n", lineNum+1, op->str);
			runStatus -> errNum ++;
			return FALSE;
		}
	}
	else if ( op -> type == DYNAMIC)
	{
		labelAddress = getLabelAddress(runStatus , op, 0);

		if (labelAddress != -1) /*Parsing the dynamic range*/
		{
			/* Getting the value depends on cell type (data / command)*/
			if (runStatus -> finalLabelArray[labelAddress].isData)
			{
				dataAddress = runStatus -> finalLabelArray[labelAddress].memAddress - runStatus -> ic - FIRST_MEM_ADDR;
				startValueData = runStatus -> dataArray[dataAddress];
			}
			else
			{
				startValueData = getIntFromWord(runStatus -> finalLabelArray[labelAddress].word);
			}
			
			/* Parsing the specified range*/
			op -> val = getRequiredBitsFromLabel(startValueData, op -> up, op-> down);
		}
		else 
		{
			printf("ERROR: Line #%d, Label \"%s\" doesn't exist.\n", lineNum+1, op->str);
			runStatus -> errNum ++;
			return FALSE;
		}
	}
	
	return TRUE;
}

/*
Function that get the location of label array inside our "table of tags"
Input: 
	RunStatus struct
	Operand struct
	integer TRUE/FALSE if we should check extern table or not.
Output: location of label in the struct, if it is not found we will return -1
*/
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
				/*Adds reference to extern struct for further dump*/
				addExternFile(runStatus, str, op -> memAddress);
				return i;
			}
		}

	return -1;
	
}

/*
Function that gets the specified range of bits from value and return integer
Input: 
	value
	highest bit
	lowest bit
Output: integer cut down from value by specified range
*/
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
	
	/*Checking the value sign +/- */
	if ( val != (val | lsb))
	{
		val >>= down;
		/*Aggregate the value for positive*/
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
		/*Aggregate the value for negative*/
		while (i < diff)
		{
			tmp = (tmp << 1) + 1;
			i++;
		}
		tmp2 <<= diff;

		return (tmp2 | (tmp & val ));
	}

}

/*
Function that adds a specified word to MemoryDump struct
Input: 
	MemoryDump struct
	WordMemory struct - word to be added
Output: -
*/
void addToMem(MemoryDump *memStatus, WordMemory word)
{	
	if( memStatus -> wordCount >= MAX_DATA_SIZE )
	{
		return ; /*We exceeded the allowed memory size*/
	}

	memStatus -> memArray[memStatus -> wordCount] = getIntFromWord(word);
	memStatus -> wordCount ++;
}



