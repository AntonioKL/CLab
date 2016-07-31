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

/*
Function that exports the obect file from the dump of the memory
Input: 
	RunStatus Struct
	MemoryDump Struct
	filename
Output: -
*/
void exportObject(RunStatus *runStatus, MemoryDump *memStatus, char *fileName)
{
	int i;
	FILE *objFile;
	objFile = openFile(fileName, OBJECT_FILE_EXT, MODE_WRITE_ONLY);

	/*Translating Adress size to special 8 base*/
	specialBase8Print(objFile,runStatus -> ic);
	fprintf(objFile, " ");

	/*Translating Data size to special 8 base*/
	specialBase8Print(objFile,runStatus -> dataCount);
	fprintf(objFile, "\n");
	
	/*Translating  to lines of Address 	Value*/
	/*In special Base8*/
	for (i=0; i < memStatus -> wordCount; i++)
	{
		/*Adress Translation*/
		specialBase8Print(objFile, i + FIRST_MEM_ADDR);
		fprintf(objFile, "\t\t\t");
		/*Value Translation , with a word size*/
		specialBase8DefinedSizePrint(objFile, memStatus -> memArray[i]);
		fprintf(objFile, "\n");
	}

	printf("Success: Created %s%s\n", fileName, OBJECT_FILE_EXT);
	fclose(objFile);

}

/*
Function that exports to extern file from extern dump
Input: 
	RunStatus Struct
	filename
Output: -
*/
void exportExtern(RunStatus *runStatus, char *fileName)
{
	int i;
	FILE *extFile;

	/*Creating only if there is a need for that*/
	if(runStatus -> externFileCount)
	{
		extFile = openFile(fileName, EXTERN_FILE_EXT, MODE_WRITE_ONLY);
	}

	/*Dump of the extern reference struct to file*/
	for (i=0; i < runStatus -> externFileCount; i++)
	{
		fprintf(extFile, "%s\t\t\t", runStatus -> externFileArray[i].name);
		specialBase8Print(extFile,runStatus -> externFileArray[i].memAddress);
		fprintf(extFile, "\n");
		
	}
	
	/*Closing only if we opened the file for write*/
	if(runStatus -> externFileCount)
	{
		printf("Success: Created %s%s\n", fileName, EXTERN_FILE_EXT);
		fclose(extFile);
	}
}

/*
Function that exports to entry file from entry dump
Input: 
	RunStatus Struct
	filename
Output: -
*/
void exportEntry(RunStatus *runStatus, char *fileName)
{
	int i;
	FILE *entFile;

	/*Creating only if there is a need for that*/
	if(runStatus -> entryCount)
	{
		entFile = openFile(fileName, ENTRY_FILE_EXT, MODE_WRITE_ONLY);
	}	

	/*Dump of the entries to file*/
	for (i=0; i < runStatus -> entryCount; i++)
	{
		fprintf(entFile, "%s\t\t\t", runStatus -> entryArray[i].name);
		specialBase8Print(entFile,runStatus -> entryArray[i].memAddress);
		fprintf(entFile, "\n");
	}
	
	/*Closing only if we opened the file for write*/
	if(runStatus -> entryCount)
	{
		printf("Success: Created %s%s\n", fileName, ENTRY_FILE_EXT);
		fclose(entFile);
	}
}

/*
Function that prints to specified file, the provided integer in special base8
Input: 
	File pointer
	integer value
Output: -
*/
void specialBase8Print(FILE *extFile, int memAddress)
{
	char buffer[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1] = "\0";
	/*Converting to special base8*/
	specialBase8ConvertInt(memAddress, buffer);
	fprintf(extFile, "%s", buffer);

}

/*
Function that prints to specified file, the provided integer in special base8 but keeps the mimimum word size required for the language
Input: 
	File pointer
	integer value
Output: -
*/
void specialBase8DefinedSizePrint(FILE *extFile, int memAddress)
{
	int i, j;
	/*Getting the size of the number words in special base8*/
	int codeSize = (MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1;
	char buffer[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1] = "\0";
	char codeStr[(MEM_WORD_SIZE)/(NUM_BITS_BASE8) + 1];
	char *baseDigits = SPECIAL_BASE8_DIGITS;
	
	/*Converting the number to special base8*/
	specialBase8ConvertInt(memAddress, buffer);

	/*Placing ! inside unused cells*/
	for ( i = 0 ; i < codeSize - strlen(buffer) -1 ; i++)
	{
		codeStr[i] = baseDigits[0];
	}
	/*Placcing the value of buffer in the rest of the cells*/
	for (j = 0 ; j < strlen(buffer) ; j++)
	{
		codeStr[i+j] = buffer[j];
	}
	/*It is a string so we have to close it with Null char*/
	codeStr[i+j] = '\0';
	fprintf(extFile, "%s", codeStr);
}

/*
Function that converts integer to special base8 and stores it in a provided string
Input: 
	integer value
	string
Output: -
*/
void specialBase8ConvertInt(int num, char *buffer)
{

	char *digits = SPECIAL_BASE8_DIGITS;
	int i = -1 ;
	int tmp_num = num;
	
	/*speical case num = 0*/
	if(!num)
	{
		buffer[0] = digits[0];
	}
	
	/*Counting the number of digits required*/
	while (tmp_num)
	{
		i++;
		tmp_num /= BASE_NUM;
	}

	/*Storing the base8 in stting*/
	while (i >= 0)
	{
		buffer[i] = digits[num % BASE_NUM];
		num /= BASE_NUM;
		i--;
	}
}












