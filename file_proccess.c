/*
=====================================================================================================================
File processing file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the reading of the input files
=====================================================================================================================
*/

#include "main_header.h"

void parseFile(char *fileName)
{	
	FILE *file = openInputFile(fileName, INPUT_FILE_EXTENSION, MODE_READ_ONLY);

	if (file == NULL)
	{
		printf("ERROR: Cannot open the file: %s"INPUT_FILE_EXTENSION"\n", fileName);
		return;
	}
	fileReadProccessManager(file, fileName);
	fclose(file);
}


FILE* openInputFile(const char *fileName, const char *extension, const char *openMode)
{
	
	char fullFileName[MAX_FILENAME_LEN + MAX_FILE_EXTENSION_LEN];
	FILE *file;
	strcpy(fullFileName, fileName);
	strcat(fullFileName, extension);
	
	
	file = fopen(fullFileName, openMode);

	return file;
}

void fileReadProccessManager(FILE *file, char *fileName)
{
	RunStatus runStatus;
	MemoryDump memStatus;
	int errNum = 0;
	
	initializeRunStatus(&runStatus);
	
	
	errNum += firstReadManager(&runStatus, file);
	if (runStatus.flagFatalErr)
	{
		releaseRunStatusStruct(&runStatus);
		return ;
	}
	
	buildFinalLabes(&runStatus);
	initializeMemoryStatus(&memStatus);

	/*Setting pointer to the start
	fseek(file, 0, SEEK_SET);
	
	resetRunParams(&runStatus);*/
	errNum += SecondReadManager(&runStatus, &memStatus);

	if (errNum > 0)
	{
		releaseRunStatusStruct(&runStatus);
		return ;
	}

	fileOutputmanager(&runStatus, fileName);
	releaseRunStatusStruct(&runStatus);
}

void initializeRunStatus(RunStatus *runStatus)
{
	runStatus -> line = NULL;
	runStatus -> originalLine = NULL;
	runStatus -> errNum = 0;
	runStatus -> flagFatalErr = FALSE;
	
	runStatus -> isLineErr = FALSE;
	runStatus -> ic = 0;
	
	runStatus -> lineArray = NULL;
	runStatus -> lineCount = 0;

	runStatus -> finalLabelArray = NULL;
	runStatus -> finalLabelCount = 0;

	runStatus -> labelArray = NULL;
	runStatus -> labelCount = 0;

	runStatus -> dataArray = NULL;
	runStatus -> dataCount = 0;
	
	runStatus -> entryArray = NULL;
	runStatus -> entryCount = 0;

	runStatus -> externArray = NULL;
	runStatus -> externCount = 0;
	
	runStatus -> externFileArray = NULL;
	runStatus -> externFileCount = 0;
	
}

void resetRunParams(RunStatus *runStatus) /**/
{
	return ;
}
void releaseRunStatusStruct(RunStatus *runStatus)
{
	/* free the data in struct that was allocated by malloc*/
	int i;

	for (i=0; i < runStatus -> lineCount; i++)
	{
		if (runStatus -> lineArray[i].op1)
		{
			free(runStatus -> lineArray[i].op1);
		}
		if (runStatus -> lineArray[i].op2)
		{
			free(runStatus -> lineArray[i].op2);
		}
	}
	if (runStatus -> lineArray)
	{
		free (runStatus -> lineArray);
	}
	if (runStatus -> finalLabelArray)
	{
		free (runStatus -> finalLabelArray);
	}
	if (runStatus -> labelArray)
	{
		free (runStatus -> labelArray);
	}
	if (runStatus -> dataArray)
	{
		free (runStatus -> dataArray);
	}
	if (runStatus -> entryArray)
	{
		free (runStatus -> entryArray);
	}
	if (runStatus -> externArray)
	{
		free (runStatus -> externArray);
	}
	if (runStatus -> externFileArray)
	{
		free (runStatus -> externFileArray);
	}
	
}

void buildFinalLabes(RunStatus *runStatus)
{
	int i=0;
			
	for (i = 0; (runStatus -> labelCount) > i; i++)
	{
		
		addLabelFinal(runStatus, runStatus -> labelArray[i].name, runStatus -> labelArray[i].memAddress + runStatus -> ic);
		
	}
	/* Debug Print
	for (i = 0; (runStatus -> finalLabelCount) > i; i++)
	{
		
		printf("-%s----%d-\n", runStatus -> finalLabelArray[i].name, runStatus -> finalLabelArray[i].memAddress);
		
	}
	*/
	/*for (i = 0; (runStatus -> entryCount) > i; i++)
	{
		
		printf("-%s---\n", runStatus -> entryArray[i].name);
		
	}*/
}


void initializeMemoryStatus(MemoryDump *memStatus)
{
	memStatus -> wordCount = 0;
}

/***/
void fileOutputmanager(RunStatus *runStatus, char *fileName)
{
	return ;
}

