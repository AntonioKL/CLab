
/*
=====================================================================================================================
File proccessing - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for reading the file and managig the reads and output
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
FILE* openFile(const char *fileName, const char *extension, const char *openMode);
void fileReadProccessManager(FILE *file, char *fileName);
void initializeRunStatus(RunStatus *runStatus);
void releaseRunStatusStruct(RunStatus *runStatus);
void buildFinalLabes(RunStatus *runStatus);
void initializeMemoryStatus(MemoryDump *memStatus);




