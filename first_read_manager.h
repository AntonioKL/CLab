
/*
=====================================================================================================================
FirstReadManager - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for first read of the file
=====================================================================================================================
*/

/*-----------------
--- Local const ---
-------------------*/

/*It was told we can assume  that the input is no longer than 80 chars, Just in case using 100 chars*/
#define 	ALLOWED_MAX_LINE_LENGTH		80
#define 	MAX_LINE_LENGTH			ALLOWED_MAX_LINE_LENGTH + 20


/*------------------------------
--- Declaration of Functions ---
--------------------------------*/


int firstReadManager(RunStatus *runStatus, FILE *file);
int readLine(FILE *file, char *buffer, int max_len);
void lineProccessor(RunStatus *runStatus);
void scanDirective(RunStatus *runStatus, char *label);
void firstParseCmd(RunStatus *runStatus, char *label);
void parseCmdOperands(RunStatus *runStatus, char *label, int cmdId);
void opProccessing(RunStatus *runStatus, char *label, int cmdId ,char *op1, char *op2);
int isLegalOperands(RunStatus *runStatus, int cmdId);

/*-----------------------------------
--- Parsing functions declaration ---
-------------------------------------*/

void parseDataDirective(RunStatus *runStatus, char *label);
void parseStringDirective(RunStatus *runStatus, char *label);
void parseExternDirective(RunStatus *runStatus, char *label);
void parseEntryDirective(RunStatus *runStatus, char *label);



