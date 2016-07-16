/*
=====================================================================================================================
File processing file
	Author: Anton Kuligin 
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
	int errNum = 0;
	
	initializeRunStatus(&runStatus);
	
	errNum = firstReadManager(&runStatus, file);
	if (errNum > 0)
	{
		return ;
	}
	
	/*Setting pointer to the start*/
	fseek(file, 0, SEEK_SET);
	
	resetRunParams(&runStatus);

	errNum = SecondReadManager(&runStatus, file);
	if (errNum > 0)
	{
		return ;
	}

	fileOutputmanager(&runStatus, fileName);
	releaseRunStatusStruct(&runStatus);
	
	
}

void initializeRunStatus(RunStatus *runStatus)
{
	return ;
}

void resetRunParams(RunStatus *runStatus)
{
	return ;
}
void releaseRunStatusStruct(RunStatus *runStatus)
{
	/* free the data in struct that was allocated by malloc*/
	/*free(pointer_to_malloc);*/
}

/*Not here*/
int firstReadManager(RunStatus *runStatus, FILE *file)
{
	return 2;
}
int SecondReadManager(RunStatus *runStatus, FILE *file)
{
	return 0;
}
void fileOutputmanager(RunStatus *runStatus, char *fileName)
{
	return ;
}

