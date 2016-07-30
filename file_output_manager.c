/*
=====================================================================================================================
FileOutputManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the Output for the files after the two runs
=====================================================================================================================
*/
#include "main_header.h"

/*
Function that handles various file dumps
Input: 
	RunStatus Struct
	MemoryDump Struct
	filename
Output: -
*/
void fileOutputmanager(RunStatus *runStatus, MemoryDump *memStatus, char *fileName)
{
	exportObject(runStatus, memStatus, fileName);
	exportExtern(runStatus, fileName);
	exportEntry(runStatus, fileName);
}

void exportObject(RunStatus *runStatus, MemoryDump *memStatus, char *fileName)
{
	int i;
	FILE *objFile;

	objFile = openFile(fileName, OBJECT_FILE_EXT, MODE_WRITE_ONLY);
	specialBase8Print(objFile,runStatus -> ic);
	fprintf(objFile, " ");
	specialBase8Print(objFile,runStatus -> dataCount);
	fprintf(objFile, "\n");
	
	for (i=0; i < memStatus -> wordCount; i++)
	{
		specialBase8Print(objFile, i + FIRST_MEM_ADDR);
		fprintf(objFile, "\t\t\t");
		specialBase8DefinedSizePrint(objFile, memStatus -> memArray[i]);
		fprintf(objFile, "\n");
	}

	printf("Success: Created %s%s\n", fileName, OBJECT_FILE_EXT);
	fclose(objFile);

}


void exportExtern(RunStatus *runStatus, char *fileName)
{
	int i;
	FILE *extFile;
	
	if(runStatus -> externFileCount)
	{
		extFile = openFile(fileName, EXTERN_FILE_EXT, MODE_WRITE_ONLY);
	}	

	for (i=0; i < runStatus -> externFileCount; i++)
	{
		fprintf(extFile, "%s\t\t\t", runStatus -> externFileArray[i].name);
		specialBase8Print(extFile,runStatus -> externFileArray[i].memAddress);
		fprintf(extFile, "\n");
		
	}
	
	if(runStatus -> externFileCount)
	{
		printf("Success: Created %s%s\n", fileName, EXTERN_FILE_EXT);
		fclose(extFile);
	}
}


void exportEntry(RunStatus *runStatus, char *fileName)
{
	int i;
	FILE *entFile;

	if(runStatus -> entryCount)
	{
		entFile = openFile(fileName, ENTRY_FILE_EXT, MODE_WRITE_ONLY);
	}	

	for (i=0; i < runStatus -> entryCount; i++)
	{
		fprintf(entFile, "%s\t\t\t", runStatus -> entryArray[i].name);
		specialBase8Print(entFile,runStatus -> entryArray[i].memAddress);
		fprintf(entFile, "\n");
	}
	
	if(runStatus -> entryCount)
	{
		printf("Success: Created %s%s\n", fileName, ENTRY_FILE_EXT);
		fclose(entFile);
	}
}

void specialBase8Print(FILE *extFile, int memAddress)
{
	char buffer[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1] = "\0";
	specialBase8ConvertInt(memAddress, buffer);
	fprintf(extFile, "%s", buffer);

}

void specialBase8DefinedSizePrint(FILE *extFile, int memAddress)
{
	int i, j;
	int codeSize = (MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1;
	char buffer[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1] = "\0";
	char codeStr[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1];
	char *baseDigits = SPECIAL_BASE8_DIGITS;
	
	specialBase8ConvertInt(memAddress, buffer);

	for ( i = 0 ; i < codeSize - strlen(buffer) -1 ; i++)
	{
		codeStr[i] = baseDigits[0];
	}
	for (j = 0 ; j < strlen(buffer) ; j++)
	{
		codeStr[i+j] = buffer[j];
	}
	codeStr[i+j] = '\0';
	fprintf(extFile, "%s", codeStr);
}


void specialBase8ConvertInt(int num, char *buffer)
{

	char *digits = SPECIAL_BASE8_DIGITS;
	int i = -1 ;
	int tmp_num = num;
	
	if(!num)
	{
		buffer[0] = digits[0];
	}

	while (tmp_num)
	{
		i++;
		tmp_num /= BASE_NUM;
	}

	while (i >= 0)
	{
		buffer[i] = digits[num % BASE_NUM];
		num /= BASE_NUM;
		i--;
	}
}












