/*
=====================================================================================================================
File processing file
	Author: Anton Kuligin 
	Description: Handles the reading of the input files
=====================================================================================================================
*/

#include "file_proccess.h"

void parseFile(char *fileName)
{
	FILE *file = openInputFile(fileName, INPUT_FILE_EXTENSION, "r");

	if (file == NULL)
	{
		printf("ERROR: Cannot open the file \"%s.as\".\n", fileName);
		return;
	}
	fclose(file);
}


FILE* openInputFile(char *fileName, char *extension, const char *openMode)
{
	char fullFileName[MAX_FILENAME_LEN + MAX_FILE_EXTENSION_LEN];
	FILE *file;
	strcpy(fullFileName, fileName);
	strcat(fullFileName, extension);
	
	file = fopen(fullFileName, openMode);

	return file;
}
