
/*
=====================================================================================================================
File proccessing - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for reading the input files
=====================================================================================================================
*/


/*-----------------
--- Local const ---
-------------------*/

#define		MODE_READ_ONLY			"r"



/*------------------------------
--- Declaration of Functions ---
--------------------------------*/

void parseFile(char* fileName);
FILE* openInputFile(const char *fileName, const char *extension, const char *openMode);
void fileReadProccessManager(FILE *file, char *fileName);
void initializeRunStatus(RunStatus *runStatus);
void resetRunParams(RunStatus *runStatus);
void releaseRunStatusStruct(RunStatus *runStatus);
/*Not here*/
int SecondReadManager(RunStatus *runStatus, FILE *file);
void fileOutputmanager(RunStatus *runStatus, char *fileName);








