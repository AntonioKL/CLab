
/*
=====================================================================================================================
File proccessing - Headers file
	Author: Anton Kuligin 
	Description: Headers for reading the input files
=====================================================================================================================
*/


/*-----------------
--- Local const ---
-------------------*/
#define 	INPUT_FILE_EXTENSION		".as"
#define		MODE_READ_ONLY			"r"


/*----------------------------
--- Declaration of Structs ---
------------------------------*/

typedef struct{
	int lineCount;
	char *line;
	int errNum;
	

} RunStatus;

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








