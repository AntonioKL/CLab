
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

/*It was told we can assume  that the input is no longer than 80 chars, Just in case I took 100 chars*/
#define 	ALLOWED_MAX_LINE_LENGTH		80
#define 	MAX_LINE_LENGTH			ALLOWED_MAX_LINE_LENGTH + 20

/*----------------------------
--- Declaration of Structs ---
------------------------------*/

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/


int firstReadManager(RunStatus *runStatus, FILE *file);
int readLine(FILE *file, char *buffer, int max_len);
void lineProccessor(RunStatus *runStatus);
void firstParseCmd(RunStatus *runStatus, char *label);

/**/
void directiveManager();








