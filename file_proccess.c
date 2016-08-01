/*
=====================================================================================================================
File processing file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Handles the reading of the input files and dividing the tasks
=====================================================================================================================
*/

#include "main_header.h"

/*
Function that used to open the desired file and parse it.
Input: File Name that should be parsed 
Output: - 
*/
void parseFile(char *fileName)
{	
	FILE *file = openFile(fileName, INPUT_FILE_EXTENSION, MODE_READ_ONLY);

	if (file == NULL)
	{
		printf("ERROR: Cannot open file: %s"INPUT_FILE_EXTENSION"\n", fileName);
		return;
	}

	/*File is open we can parse it now*/
	fileReadProccessManager(file, fileName);
	fclose(file);
}

/*
Function that opens the file and returns it.
Input: 
	File Name
	Extension of the File
	MODE : ( READ, Write , Append)
Output: Pointer to the start of the file
*/

FILE* openFile(const char *fileName, const char *extension, const char *openMode)
{
	
	char fullFileName[MAX_FILENAME_LEN + MAX_FILE_EXTENSION_LEN];
	FILE *file;
	strcpy(fullFileName, fileName);
	strcat(fullFileName, extension);
	
	file = fopen(fullFileName, openMode);
	return file;
}

/*
Function that manages the reading process of the file and manages the output.
Input: 
	File to work on
	filename string

Output: -
*/
void fileReadProccessManager(FILE *file, char *fileName)
{
	RunStatus runStatus;
	MemoryDump memStatus;
	int errNum = 0;
	
	initializeRunStatus(&runStatus);
	errNum += firstReadManager(&runStatus, file);

	/*We stop in case of failed reallocation of memory in realloc*/
	if (runStatus.flagFatalErr)
	{
		releaseRunStatusStruct(&runStatus);
		printf("ERROR: Fatal Error, cannot continue.\n");
		return ;
	}
	
	/*If we have errors in first run, we will release the allocated data and print an information about it*/
	if (errNum)
	{
		releaseRunStatusStruct(&runStatus);
		printf("\nFail: You have %d errors, please fix them\n", errNum);
		return ;
	}

	/*Creating the full "TAVLAT SMALIM" from Label of Data combined with Label of commands*/
	buildFinalLabes(&runStatus);
	initializeMemoryStatus(&memStatus);

	/*Second run now we are dumping to memStatus struct*/
	errNum = SecondReadManager(&runStatus, &memStatus);

	/*If we have errors in second run, we will release the allocated data and print an information about it*/
	if (!errNum)
	{
		/*Passing the status of the run and the dump from memory to output manager*/
		fileOutputmanager(&runStatus, &memStatus, fileName);
		printf("Success: The file has been parsed\n\n");
	}
	else
	{
		printf("\nFail: You have %d errors, please fix them\n", errNum);
	}

	/*Releasing all Dynamically allocated data*/
	releaseRunStatusStruct(&runStatus);
}

/*
Function that initialize the parameters in the struct. The initializing is required for the run.
Input: 
	RunStaus Struct
Output: -
*/
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

/*
Function that releases the parameters in the struct.
Input: 
	RunStaus Struct
Output: -
*/
void releaseRunStatusStruct(RunStatus *runStatus)
{
	/*
	 free the data in struct that was allocated by malloc/realloc
	 The data in struct is set free only it was reallocated from NULL
	*/
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

/*
Function that builds the "Table of Signs", from data labels and command labels by right order
Input: 
	RunStaus Struct
Output: -
*/
void buildFinalLabes(RunStatus *runStatus)
{
	int i=0;
	
	for (i = 0; (runStatus -> labelCount) > i; i++)
	{
		
		addLabelFinal(runStatus, runStatus -> labelArray[i].name, runStatus -> labelArray[i].memAddress + runStatus -> ic, TRUE);
		
	}
}

/*
Function that initializes the parameters inside MemoryDump Struct
Input: 
	MemoryDump Struct
Output: -
*/
void initializeMemoryStatus(MemoryDump *memStatus)
{
	memStatus -> wordCount = 0;
}


