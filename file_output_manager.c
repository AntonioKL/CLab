/*
=====================================================================================================================
FileOutputManager
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the Output for the files after the two runs
=====================================================================================================================
*/
#include "main_header.h"

void fileOutputmanager(RunStatus *runStatus, MemoryDump *memStatus, char *fileName)
{
	/*exportObject();
	printf("Success: Created %s%s\n", fileName, OBJECT_FILE_EXT);*/
	exportExtern(runStatus, fileName);
	printf("Success: Created %s%s\n", fileName, ENTRY_FILE_EXT);
	/*exportEntry();*/
	printf("Success: Created %s%s\n", fileName, EXTERN_FILE_EXT);
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
		fclose(extFile);
	}
}

void specialBase8Print(FILE *extFile, int memAddress)
{
	char buffer[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1] = "\0";
	specialBase8ConvertInt(memAddress, buffer, 0);
	fprintf(extFile, "%s", buffer);

}


void specialBase8ConvertInt(int num, char *buffer, int index)
{

	char *digits = SPECIAL_BASE8_DIGITS;
	int i = -1 ;
	int tmp_num = num;
	
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












